#include "BaseGameActor.h"
#include "Field.h"
#include "FieldPiece.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "PanelCutLocus.h"
#include "UnableThroughEdge.h"
#include "UnableThroughBlock.h"
#include "CircularSaw.h"
#include "ActorManager.h"
#include "DrawMode.h"
#include "Input.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "ItemEmitter.h"
#include "IActionState.h"
#include "ActionStateMove.h"
#include "ActionStateTackle.h"
#include "ActionStateBlown.h"
#include "ActionStateWithstand.h"
#include "ActionStateCut.h"
#include "ActionStateFall.h"
#include "Easing.h"

ComPtr<ID3D12Resource> BaseGameActor::constCameraBuff = nullptr;
//DebugCamera* BaseGameActor::camera = nullptr;
InGameCamera* BaseGameActor::camera = nullptr;
bool BaseGameActor::rotCamera = false;
float BaseGameActor::radY = 0;
int BaseGameActor::cameraRotCount = 0;
const int BaseGameActor::ROT_TIME = 10;

BaseGameActor::BaseGameActor(const Vector3& arg_pos)
	 :startPos(arg_pos),
	 RADIUS(1.0f),
	 weight(10),
	 prePos(arg_pos),
	 virtualityPlanePosition(arg_pos),
	 preVirtualityPlanePosition(arg_pos),
	 direction(Vector3(0, 0, 1)),
	 isCrushed(false),
	 isEndGame(false),
	 actionState(ActionStateMove::GetInstance()),
	 killCount(0),
	 isInFever(false),
	 feverTimer(new Timer(5 * 60)),
	 speed(0.18f),
	 WALK_SPEED(0.18f),
	 DRAWING_SPEED(0.36f),
	 BLOWN_SPEED(0.50f),
	 ROTATE_SPEED(17.5f),
	 isHitDuringTackle(false),
	 tackleStartPos(Vector3()),
	 tackleEndPos(Vector3()),
	 tackleCount(0),
	 blownTime(0),
	 pHitActor(nullptr),
	 hitCount(0),
	 withstandTime(0),
	 preWithstandVec(Vector3()),
	 BGColor(1),
	 isReturningField(false),
	 returningStartPos(Vector3()),
	 returningEndPos(Vector3()),
	 moveEasingCount(0),
	 inputStartCount(0),
	 nextInputStartCount(40),
	 notWithstandCount(0),
	 cutPower(0),
	 gottenPanel(0),
	 bonusCount(0),
	 targetActor(nullptr),
	 targetIndex(-1),
	 fallEasingCount(0),
	 fallStartPos(Vector3()),
	 fallEndPos(Vector3()),
	 isPlayedFallSound(false),
	 respawnPiece(nullptr)
{
	position = arg_pos;
	pObjectManager = ObjectManager::GetInstance();

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, XMFLOAT4(1, 1, 0, 0.6f));
	panelCutLocus->SetParentObject(this);
	weightInfluenceMap.clear();
}

BaseGameActor::~BaseGameActor()
{
	delete panelCountSprite3D;
	delete dropPointGetUI;
	delete feverTimer;
}

void BaseGameActor::Initialize()
{
	scale = { 0.9f };
	rotation = 0;
	position = startPos;
	weight = 10;
	prePos = position;
	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	direction = Vector3(0, 0, 1);
	panelCountSprite3D->Initialize();
	isCrushed = false;
	isEndGame = false;
	actionState = ActionStateMove::GetInstance();
	actionState->Initialize(this);
	killCount = 0;
	dropPointGetUI->Initialize();
	weightInfluenceMap.clear();
	isInFever = false;
	feverTimer->Reset();
	speed = WALK_SPEED;
	isHitDuringTackle = false;
	tackleStartPos = Vector3();
	tackleEndPos = Vector3();
	tackleCount = 0;
	blownTime = 0;
	pHitActor = nullptr;
	hitCount = 0;
	withstandTime = 0;
	preWithstandVec = Vector3();
	BGColor = 1;
	isReturningField = false;
	returningStartPos = Vector3();
	returningEndPos = Vector3();
	moveEasingCount = 0;
	inputStartCount = 0;
	nextInputStartCount = 40;
	notWithstandCount = 0;
	panelCutLocus->SetCutPower(0);
	panelCutLocus->Move(Vector3(), 0);
	cutPower = 0;
	gottenPanel = 0;
	bonusCount = 0;
	targetActor = nullptr;
	targetIndex = -1;
	fallEasingCount = 0;
	fallStartPos = Vector3();
	fallEndPos = Vector3();
	isPlayedFallSound = false;
	respawnPiece = nullptr;
}

void BaseGameActor::Update()
{
	if (isEndGame)
	{
		return;
	}

	if (isCrushed)
	{
		static int pressCount = 0;
		pressCount++;
		if (pressCount >= 30)
		{
			isEndGame = true;
		}
	}
	else
	{
		if (isInFever)
		{
			feverTimer->Update();
			cutPower = 6;

			if (feverTimer->IsTime() && actionState->GetLabel() != ActionStateLabel::CUT)
			{
				cutPower = 0;
				isInFever = false;
				feverTimer->Reset();
			}
		}

		Field* field = ActorManager::GetInstance()->GetFields()[0];
		CuttingInfo* info = field->GetCuttingInfo(this);

		SetTargetActor();
		IActionState* state = actionState->Update(this);
		if (state != actionState)
		{
			ChangeActionState(actionState, state);
			actionState = state;
		}

		field->DecideCuttingInfo(this, virtualityPlanePosition, direction);

		if (actionState->GetLabel() != ActionStateLabel::CUT)
		{
			panelCutLocus->SetCutPower(cutPower);
		}
		panelCutLocus->Move(info->cuttingStartPos, info->cuttingAngle);

		if (isInFever)
		{
			if (feverTimer->GetTime() % 3 == 0)
			{
				ParticleEmitter::FeverEffect(position);
			}

		}
	}

	Object::Update();
	panelCountSprite3D->Update();
	dropPointGetUI->Update();
}

void BaseGameActor::Draw()
{
	if (!Object3D::GetDrawShadow())
	{
		HRESULT result;

		//定数バッファをセット
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constCameraBuff->GetGPUVirtualAddress());
	}
	CustomDraw(true, true);
	
	dropPointGetUI->Draw();
	panelCountSprite3D->Draw();
}

void BaseGameActor::DrawReady()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
		XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
		Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
		cameraDirectionZ.Normalize();
		ImGui::Begin("PlayerStatus");
		ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
		ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
		ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
		ImGui::Text("Rot : {%f, %f, %f }\n", rotation.x, rotation.y, rotation.z);
		ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
		ImGui::End();
	}
#endif

	panelCountSprite3D->Draw();

	if (Object3D::GetDrawShadow())
	{
		pipelineName = "FBXShadowMap";
	}
	else
	{
		pipelineName = "FBX";
	}
}

void BaseGameActor::InitCamera(const Vector3& arg_pos)
{
	//定数バッファの作成
	HRESULT result;
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constCameraBuff));
	assert(SUCCEEDED(result));

	//デプスシャドウ用のカメラ情報を設定
	ConstLightCameraBuff* constMap2 = nullptr;
	result = constCameraBuff->Map(0, nullptr, (void**)&constMap2);
	assert(SUCCEEDED(result));
	constMap2->cameraPos = Object3D::GetLightCamera()->GetEye();
	constMap2->viewProjection = XMMatrixLookAtLH(Vector3(Object3D::GetLightCamera()->GetEye()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetTarget()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetUp()).ConvertXMVECTOR()) * XMMatrixOrthographicLH(100, 100, 1.0f, 1000.0f);
	constCameraBuff->Unmap(0, nullptr);

	camera->SetTarget(arg_pos + Vector3{ 0, 1, 0 });

	camera->Update();
	//----------カメラの回転角度を算出---------------
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	float cosA = Vector3(0, 0, 1).Dot(cameraDirectionZ);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;
	float rad = acos(cosA);
	const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
	if (CrossVec.y < 0)
		rad *= -1;
	//-----------------------------------------------
	//カメラの回転
	//camera->AddPhi(rad);
	camera->SetTheta(120);
	camera->SetDistance(100);
}

void BaseGameActor::MoveCamera(Vector3 arg_dir)
{
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();

	//カメラのリセット処理
#ifdef _DEBUG
	if (Input::TriggerKey(DIK_C) && !rotCamera)
	{
		rotCamera = true;
		float cosA = arg_dir.Dot(cameraDirectionZ);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;
		radY = acos(cosA);
		const Vector3 CrossVec = arg_dir.Cross(cameraDirectionZ);
		if (CrossVec.y < 0)
			radY *= -1;
		cameraRotCount = 0;
		//camera->AddPhi(radY);
	}
#endif // _DEBUG

	//カメラの回転処理
	if (rotCamera)
	{
		cameraRotCount++;
		float rad = radY / ROT_TIME;
		camera->AddPhi(rad);
		if (cameraRotCount >= ROT_TIME)
			rotCamera = false;
	}
}

void BaseGameActor::StayInTheField(ActionStateLabel& arg_label)
{
	//Withstandは不実行
	if (actionState->GetLabel() == ActionStateLabel::WITHSTAND || actionState->GetLabel() == ActionStateLabel::FALL)
	{
		return;
	}

	std::vector<Vector2> fieldEdges = Field::GetEdges();

	for (int i = 0; i < fieldEdges.size(); i++)
	{
		Vector2 A = fieldEdges[i];
		Vector2 B = fieldEdges[(i + 1) % fieldEdges.size()];
		Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;
		Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - B;
		Vector2 AB = B - A;
		Vector2 normalAB = Vector2::Normalize(AB);

		//今当たっているか
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > RADIUS)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			//arg_label = ActionStateLabel::WITHSTAND;
			if (arg_label == ActionStateLabel::TACKLE)
			{
				arg_label = ActionStateLabel::MOVE;
			}
			preWithstandVec = -virtualityPlanePosition;
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			//arg_label = ActionStateLabel::WITHSTAND;
			if (arg_label == ActionStateLabel::TACKLE)
			{
				arg_label = ActionStateLabel::MOVE;
			}
			preWithstandVec = -virtualityPlanePosition;
			break;
		}

		//通り過ぎたか
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			//arg_label = ActionStateLabel::WITHSTAND;
			if (arg_label == ActionStateLabel::TACKLE)
			{
				arg_label = ActionStateLabel::MOVE;
			}
			preWithstandVec = -virtualityPlanePosition;
			break;
		}
	}
}

void BaseGameActor::StayOnRemainPieces(ActionStateLabel& arg_label, FieldPiece* arg_piece)
{
	//Withstandは不実行
	if (actionState->GetLabel() == ActionStateLabel::WITHSTAND || actionState->GetLabel() == ActionStateLabel::FALL)
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, RADIUS);

	if (piece)
	{
		virtualityPlanePosition = preVirtualityPlanePosition;

		Vector3 outPieceVec = {};
		outPieceVec = position - piece->GetVirtualityPlanePosition();
		outPieceVec.Normalize();

		arg_piece = piece;
		arg_label = ActionStateLabel::WITHSTAND;
		preWithstandVec = outPieceVec;

		return;
	}

	//即落ち
	//Withstandでないなら
	if (actionState->GetLabel() != ActionStateLabel::WITHSTAND)
	{
		auto gottenPanels = field->GetGottenPieces();
		for (auto p : gottenPanels)
		{
			bool isFall = true;

			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - p->GetVirtualityPlanePosition())) > RADIUS + FieldPiece::GetLowerTimeOffset())
			{
				continue;
			}

			auto points = p->GetPoints();
			for (int i = 0; i < points.size(); i++)
			{
				Vector2 A = points[i];
				Vector2 B = points[(i + 1) % points.size()];
				Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;
				Vector2 AB = B - A;
				Vector2 normalAB = Vector2::Normalize(AB);

				//今当たっているか
				float cross = Vector2::Cross(AO, normalAB);
				if (cross < 0)
				{
					isFall = false;
					break;
				}
			}

			if (isFall)
			{
				arg_piece = p;
				arg_label = ActionStateLabel::FALL;
				fallStartPos = virtualityPlanePosition;
				fallEndPos = p->GetVirtualityPlanePosition();				
				return;
			}
		}
	}
}

void BaseGameActor::DischargeGottenPanel(BaseGameActor* arg_actor)
{
	if (arg_actor->GetActionState()->GetLabel() == ActionStateLabel::FALL)
	{
		return;
	}

	ItemEmitter* itemEmitter = ItemEmitter::GetInstance();
	int maxEmit = 0;
	if (gottenPanel == 1)
	{
		maxEmit = 1;
	}
	else
	{
		maxEmit = gottenPanel / 2;
	}

	for (int i = 0; i < maxEmit; i++)
	{
		itemEmitter->EmitPanelItem(virtualityPlanePosition);
		gottenPanel--;
		weight -= FieldPiece::GetWeight();
	}

	if (maxEmit != 0)
	{
		//Audio::PlayWave("SE_TriangleLost", 1.0f * Audio::volume_se);
	}
}

void BaseGameActor::UpdatePos()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void BaseGameActor::ChangeActionState(IActionState* arg_current, IActionState* arg_next)
{
	arg_current->ShutDown(this);
	arg_next->Initialize(this);
}

void BaseGameActor::InFever()
{
	isInFever = true;
	feverTimer->Reset();
}

void BaseGameActor::StartMove()
{
}

void BaseGameActor::OnMove(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	speed = WALK_SPEED;

	SlidingDown();

	//移動方向
	Vector3 moveDirection = direction;
	DecideDirection(moveDirection);

	//回転処理
	//現在の進行方向とカメラの正面と角度を求める
	direction.Normalize();
	float cosA = direction.Dot(moveDirection);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;

	float rotY = acos(cosA) * 180 / 3.14159365f;
	const Vector3 CrossVec = direction.Cross(moveDirection);

	//入力がないので0にする
	if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
	{
		moveDirection = Vector3(0, 0, 0);
	}

	float rotSpeed = ROTATE_SPEED;
	if (abs(rotY) < 55)
	{
		virtualityPlanePosition += moveDirection * speed;
	}

	if (rotSpeed > abs(rotY))
	{
		rotSpeed = rotY;
	}
	if (CrossVec.y < 0)
		rotSpeed *= -1;
	rotation.y += rotSpeed;
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
	XMVECTOR dir = { direction.x,direction.y,direction.z,0 };
	dir = XMVector3TransformNormal(dir, matRot);
	direction = dir;

	UpdatePos();
}

void BaseGameActor::EndMove()
{
}

void BaseGameActor::StartTackle()
{
	//Moveからのみ実行可能
	tackleCount = 0;
	isHitDuringTackle = false;

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//スティックの向き
	auto vec = Input::GetLStickDirection();
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 16;

	Audio::PlaySE("SE_Dash", 1.0f * Audio::volume_se);
}

void BaseGameActor::OnTackle(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	speed = WALK_SPEED;

	SlidingDown();

	virtualityPlanePosition = EasingMove(tackleStartPos, tackleEndPos, 1, tackleCount / 30.0f);
	tackleCount++;
	if (tackleCount >= 30)
	{
		arg_label = ActionStateLabel::MOVE;
	}

	UpdatePos();
}

void BaseGameActor::BaseGameActor::EndTackle()
{
	tackleCount = 0;
	isHitDuringTackle = false;
	speed = WALK_SPEED;
}

void BaseGameActor::StartBlown()
{
	hitCount = 0;
}

void BaseGameActor::OnBlown(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	SlidingDown();

	speed = BLOWN_SPEED;

	virtualityPlanePosition += velocity * speed;

	if (pHitActor)
	{
		hitCount++;
		if (hitCount >= 10)
		{
			hitCount = 0;
			pHitActor = nullptr;
		}
	}

	blownTime--;
	if (blownTime <= 0)
	{
		arg_label = ActionStateLabel::MOVE;
	}

	UpdatePos();
}

void BaseGameActor::EndBlown()
{
	blownTime = 0;
}

void BaseGameActor::StartWithstand()
{
	inputStartCount = nextInputStartCount;
	withstandTime = 120;

	preWithstandVec.y = 0;
	preWithstandVec.Normalize();
}

void BaseGameActor::OnWithstand(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	arg_label = ActionStateLabel::FALL;

	//踏ん張り中　赤
	//Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	//if (BGColor >= 0)
	//{
	//	BGColor -= 0.02f;
	//}
	//else
	//{
	//	BGColor = 1;
	//}
	//if (inputStartCount <= 0)
	//{
	//	//カメラのビュー行列の逆行列を計算
	//	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	//	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	//	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	//	Vector2 stickDirection = {};
	//	//スティックの向き
	//	auto vec = Input::GetLStickDirection();
	//	stickDirection.x = (cameraDirectionX * vec.x).x;
	//	stickDirection.y = (cameraDirectionZ * vec.y).z;
	//	stickDirection = Vector2::Normalize(stickDirection);
	//	float accuracy = 0;
	//	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);
	//	accuracy = Vector2::Dot(stickDirection, correctVec);
	//	if (accuracy <= 0)
	//	{
	//		accuracy = 0;
	//	}
	//	if (accuracy >= 0.55f)
	//	{
	//		Vector3 moveDirection = preStandVec;
	//		Object::SetColor({ 1,1,1,1 });
	//		returningFieldFlag = true;
	//		standingFlag = false;
	//		returningStartPos = virtualityPlanePosition;
	//		returningEndPos = virtualityPlanePosition + moveDirection * 3;
	//		nextInputStartCount = nextInputStartCount + 30;
	//		Audio::StopWave("SE_SteppingOn");
	//		return;
	//	}
	//	standTime--;
	//	if (standTime <= 0)
	//	{
	//		Object::SetColor({ 1,1,1,1 });
	//		standingFlag = false;
	//		fallFlag = true;
	//		fallStartPos = virtualityPlanePosition;
	//		fallEndPos = virtualityPlanePosition + (-preStandVec * 4);
	//		Audio::StopWave("SE_SteppingOn");
	//	}
	//}
	//else
	//{
	//	inputStartCount--;
	//}
	//
	//if (isReturningField)
	//{
	//	virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
	//	moveEasingCount++;
	//	if (moveEasingCount >= 30)
	//	{
	//		arg_label = ActionStateLabel::MOVE;
	//	}
	//}

	UpdatePos();
}

void BaseGameActor::EndWithstand()
{
	fallStartPos = virtualityPlanePosition;
	fallEndPos = virtualityPlanePosition + (-preWithstandVec * 4);
	speed = WALK_SPEED;
}

void BaseGameActor::StartCut()
{
	CuttingInfo* info = ActorManager::GetInstance()->GetFields()[0]->GetCuttingInfo(this);
	ObjectManager::GetInstance()->Add(new CircularSaw(info->cuttingStartPos, panelCutLocus, this));
}

void BaseGameActor::OnCut(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	speed = WALK_SPEED;

	UpdatePos();
}

void BaseGameActor::EndCut()
{
}

void BaseGameActor::CompleteCut()
{
	panelCutLocus->RecordCuttedPanelPos();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	int num = field->CutPanel(panelCutLocus, bonusCount, targetActor);
	/*weight += num * FieldPiece::GetWeight();
	gottenPanel += num;*/
	if (targetActor)
	{
		if (targetActor->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			targetActor->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor->ForcedWeight(num, this);
		}
	}

	cutPower = 0;

	if (field->IsNewFeverPlayer())
	{
		InFever();
	}

	CuttingInfo* info = field->GetCuttingInfo(this);
	virtualityPlanePosition = info->ridingPiece->GetVirtualityPlanePosition();
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	ChangeActionState(actionState, ActionStateMove::GetInstance());
	actionState = ActionStateMove::GetInstance();
}

void BaseGameActor::SuspendCut()
{
	auto c = ActorManager::GetInstance()->GetCircularSaw(this);
	if (c)
	{
		c->Dead();
	}
}

void BaseGameActor::ForcedWeight(const int arg_num, BaseGameActor* arg_actor)
{
	weight += FieldPiece::GetWeight() * arg_num;
	gottenPanel += arg_num;

	auto itr = weightInfluenceMap.find(arg_actor);
	if (itr == weightInfluenceMap.end())
	{
		weightInfluenceMap.emplace(arg_actor, arg_num);
	}
	else
	{
		itr->second += arg_num;
	}
}

void BaseGameActor::StartFall()
{
	fallEasingCount = 0;
	isPlayedFallSound = false;
	isInFever = false;
	feverTimer->Reset();
}

void BaseGameActor::OnFall(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	speed = WALK_SPEED;

	if (fallEasingCount <= 30)
	{
		fallEasingCount++;
		virtualityPlanePosition = EasingMove(fallStartPos, fallEndPos, 1, fallEasingCount / 30.0f);

		Field* field = ActorManager::GetInstance()->GetFields()[0];
		position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	}
	else
	{
		virtualityPlanePosition.y -= 2;

		position = virtualityPlanePosition;
	}

	if (virtualityPlanePosition.y <= -100)
	{
		if (!isPlayedFallSound)
		{
			Audio::PlaySE("SE_Fall", 1.0f * Audio::volume_se);
			isPlayedFallSound = true;
		}
		//isEndGame = true;
		arg_label = ActionStateLabel::SPAWN;
	}
}

void BaseGameActor::EndFall()
{
	BaseGameActor* mostForcedActor = nullptr;
	for (auto m : weightInfluenceMap)
	{
		if (!mostForcedActor)
		{
			mostForcedActor = m.first;
		}
		else
		{
			if (m.second > weightInfluenceMap[mostForcedActor])
			{
				mostForcedActor = m.first;
			}
		}
	}

	if (mostForcedActor)
	{
		mostForcedActor->AddKillCount(1);
	}
}

void BaseGameActor::StartSpawn()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	respawnPiece = field->GetRespawnPiece(ObjectRegistType::PLAYER);
	virtualityPlanePosition = respawnPiece->GetVirtualityPlanePosition();
	position = respawnPiece->GetVirtualityPlanePosition();
	position.y = field->GetPosition().y + 10;

	isCrushed = false;
	isEndGame = false;
	panelCutLocus->SetCutPower(0);
	panelCutLocus->Move(Vector3(), 0);
	cutPower = 0;
	gottenPanel = 0;
}

void BaseGameActor::OnSpawn(ActionStateLabel& arg_label)
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	const float fallSpeed = 0.3f;
	position.y -= fallSpeed;

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	Vector3 tmpPos = LocusUtility::RotateForFieldTilt(respawnPiece->GetVirtualityPlanePosition(), field->GetAngleTilt(), field->GetPosition());
	if (position.y <= tmpPos.y)
	{
		arg_label = ActionStateLabel::MOVE;
	}
}

void BaseGameActor::EndSpawn()
{
	rotation = 0;
	weight = 10;
	direction = Vector3(0, 0, 1);
	panelCountSprite3D->Initialize();
	isCrushed = false;
	isEndGame = false;
	dropPointGetUI->Initialize();
	weightInfluenceMap.clear();
	isInFever = false;
	feverTimer->Reset();
	panelCutLocus->SetCutPower(0);
	panelCutLocus->Move(Vector3(), 0);
	cutPower = 0;
	gottenPanel = 0;
	targetActor = nullptr;
	targetIndex = -1;
	isPlayedFallSound = false;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	field->ChangeIsCutableWithAround(respawnPiece, true);
	respawnPiece = nullptr;
}

void BaseGameActor::HitCheckActor(BaseGameActor* arg_actor)
{
	if (pHitActor == arg_actor)
	{
		return;
	}
	else if (arg_actor->pHitActor == this)
	{
		return;
	}

	if (actionState->GetLabel() == ActionStateLabel::FALL ||
		actionState->GetLabel() == ActionStateLabel::SPAWN ||
		arg_actor->actionState->GetLabel() == ActionStateLabel::FALL ||
		arg_actor->actionState->GetLabel() == ActionStateLabel::SPAWN)
	{
		return;
	}

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(arg_actor->GetVirtualityPlanePosition() - virtualityPlanePosition));
	if (length <= RADIUS + arg_actor->RADIUS)
	{
		HitActor(arg_actor);
	}
}

void BaseGameActor::HitActor(BaseGameActor* arg_actor)
{
	//相手がFallならreturn
	if (arg_actor->GetActionState()->GetLabel() == ActionStateLabel::FALL)
	{
		return;
	}

	Audio::PlaySE("SE_Collision", 1.0f * Audio::volume_se);
	//パーティクル
	ParticleEmitter::ShockEffect((arg_actor->GetPosition() + position) / 2.0f, Vector3(255.0f, 255.0f, 255.0f));

	{
		Vector3 myVel = velocity;
		Vector3 OthersVel = arg_actor->GetVelocity();

		blownTime = 25;
		velocity = OthersVel;
		arg_actor->blownTime = 25;
		arg_actor->velocity = myVel;
	}

	if (actionState->GetLabel() == ActionStateLabel::CUT)
	{
		SuspendCut();
	}
	if (arg_actor->GetActionState()->GetLabel() == ActionStateLabel::CUT)
	{
		arg_actor->SuspendCut();
	}

	ChangeActionState(actionState, ActionStateBlown::GetInstance());
	actionState = ActionStateBlown::GetInstance();
	pHitActor = arg_actor;
	hitCount = 0;
	arg_actor->ChangeActionState(arg_actor->GetActionState(), ActionStateBlown::GetInstance());
	arg_actor->actionState = ActionStateBlown::GetInstance();
	arg_actor->pHitActor = this;
	arg_actor->hitCount = 0;
}

void BaseGameActor::HitCheckEnergyItem(EnergyItem* arg_energyItem)
{
	if (arg_energyItem->IsDead())
	{
		return;
	}

	if (!arg_energyItem->IsAppeared())
	{
		return;
	}

	if (actionState->GetLabel() == ActionStateLabel::CUT ||
		actionState->GetLabel() == ActionStateLabel::FALL ||
		actionState->GetLabel() == ActionStateLabel::SPAWN)
	{
		return;
	}

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - arg_energyItem->GetVirtualityPlanePosition()));

	if (length <= RADIUS + EnergyItem::GetRadius())
	{
		HitEnergyItem(arg_energyItem);
	}
}

void BaseGameActor::HitEnergyItem(EnergyItem* arg_energyItem)
{
	/*switch (arg_energyItem->GetRank())
	{
	case RankEnergyItem::NORMAL:
		cutPower++;
		break;
	case RankEnergyItem::SILVER:
		cutPower += 2;
		break;
	case RankEnergyItem::GOLD:
		cutPower += 3;
		break;
	default:
		cutPower++;
		break;
	}*/
	const int BONUS_COUNT_UINT = 5;
	int add = bonusCount / BONUS_COUNT_UINT + 1;
	if (add > 3)
	{
		add = 3;
	}
	cutPower += add;

	if (cutPower > 6)
	{
		cutPower = 6;
	}

	Audio::PlaySE("SE_GetTriangle", 1.0f * Audio::volume_se);
	arg_energyItem->Dead();
}

void BaseGameActor::HitCheckPanelItem(PanelItem* arg_panelItem)
{
	if (arg_panelItem->IsDead())
	{
		return;
	}

	if (!arg_panelItem->IsEndBounce())
	{
		return;
	}

	if (actionState->GetLabel() == ActionStateLabel::FALL ||
		actionState->GetLabel() == ActionStateLabel::SPAWN)
	{
		return;
	}

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - arg_panelItem->GetVirtualityPlanePosition()));

	if (length <= RADIUS + PanelItem::GetRadius())
	{
		HitPanelItem(arg_panelItem);
	}
}

void BaseGameActor::HitPanelItem(PanelItem* arg_panelItem)
{
	arg_panelItem->Dead();
	weight += FieldPiece::GetWeight();
	gottenPanel++;
}

void BaseGameActor::HitCheckUnableThroughEdge(UnableThroughEdge* arg_edge)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);
	if (!info->ridingPiece)
	{
		HitUnableThroughEdge(arg_edge);
	}
}

void BaseGameActor::HitUnableThroughEdge(UnableThroughEdge* arg_edge)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	if (arg_edge->IsEndFallDown())
	{
		virtualityPlanePosition = preVirtualityPlanePosition;
		position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	}
	else if (arg_edge->GetPosition().y <= field->GetPosition().y + UnableThroughEdge::GetHeightMagnification() * 2)
	{
		isCrushed = true;
	}
}

void BaseGameActor::HitCheckUnableThroughBlock(UnableThroughBlock* arg_block)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);

	if (info->ridingPiece == arg_block->GetParentPiece())
	{
		HitUnableThroughBlock(arg_block);
	}
}

void BaseGameActor::HitUnableThroughBlock(UnableThroughBlock* arg_block)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	if (arg_block->IsEndFallDown())
	{
		virtualityPlanePosition = preVirtualityPlanePosition;
		position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	}
	else if (arg_block->GetVirtualityPlanePosition().y <= field->GetPosition().y + UnableThroughBlock::GetHeightMagnification() * 2)
	{
		isCrushed = true;
	}
}

void BaseGameActor::SlidingDown()
{
	//Move,Tackle,Blowで実行
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.05f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;

	if (nextInputStartCount > 60)
	{
		notWithstandCount++;
		if (notWithstandCount >= 60)
		{
			notWithstandCount = 0;
			nextInputStartCount -= 2;
		}
	}
}

Vector3 BaseGameActor::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	Vector3 result = {};
	result.x = Easing::EaseOutCubic(arg_startPos.x, arg_endPos.x, arg_maxTime, arg_nowTime);
	result.y = Easing::EaseOutCubic(arg_startPos.y, arg_endPos.y, arg_maxTime, arg_nowTime);
	result.z = Easing::EaseOutCubic(arg_startPos.z, arg_endPos.z, arg_maxTime, arg_nowTime);
	return result;
}

void BaseGameActor::DecideDirection(Vector3& arg_direction)
{
	if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
	{
		return;
	}

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//くり抜き動作中
	if (actionState->GetLabel() == ActionStateLabel::CUT)
	{
		arg_direction = { 0,0,0 };
	}
	else
	{
		if (Input::DownKey(DIK_A))
			arg_direction += cameraDirectionX * -1;
		if (Input::DownKey(DIK_D))
			arg_direction += cameraDirectionX;
		if (Input::DownKey(DIK_S))
			arg_direction += cameraDirectionZ * -1;
		if (Input::DownKey(DIK_W))
			arg_direction += cameraDirectionZ;
		if (Input::CheckPadLStickAnythingDir())
		{
			auto vec = Input::GetLStickDirection();

			arg_direction = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
		}
	}

	arg_direction.Normalize();
	//反発用に代入
	velocity = arg_direction;
}

void BaseGameActor::SetTargetActor()
{
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();

	if (!targetActor)
	{
		for (int i = 0; i < actors.size(); i++)
		{
			if (actors[i] == this)
			{
				continue;
			}

			if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
				actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
			{
				targetActor = actors[i];
				targetIndex = i;
				break;
			}
		}
	}
	else
	{
		if (targetActor->GetActionState()->GetLabel() == ActionStateLabel::FALL ||
			targetActor->GetActionState()->GetLabel() == ActionStateLabel::SPAWN)
		{
			targetActor = nullptr;
			targetIndex = -1;
			for (int i = 0; i < actors.size(); i++)
			{
				if (actors[i] == this)
				{
					continue;
				}

				if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
					actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
				{
					targetActor = actors[i];
					targetIndex = i;
					break;
				}
			}
		}
	}

	if (!targetActor)
	{
		return;
	}

	if (!Input::TriggerPadRightTrigger())
	{
		return;
	}

	//現在のtargetEnemyより後ろ
	for (int i = targetIndex + 1; i < actors.size(); i++)
	{
		if (actors[i] == this)
		{
			continue;
		}

		if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor = actors[i];
			targetIndex = i;
			return;
		}
	}
	//現在のtargetEnemyより前
	for (int i = 0; i < targetIndex; i++)
	{
		if (actors[i] == this)
		{
			continue;
		}

		if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor = actors[i];
			targetIndex = i;
			return;
		}
	}
}

bool BaseGameActor::IsChangeMoveToTackle()
{
	//return Input::TriggerPadButton(XINPUT_GAMEPAD_B);
	return false;
}

bool BaseGameActor::IsChangeMoveToCut()
{
	CuttingInfo* info = ActorManager::GetInstance()->GetFields()[0]->GetCuttingInfo(this);
	return Input::TriggerPadButton(XINPUT_GAMEPAD_A) && cutPower > 0 && info->ridingPiece;
}