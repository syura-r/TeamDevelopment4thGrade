#include "BaseGameActor.h"
#include "Field.h"
#include "FieldPiece.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "PanelCutLocus.h"
#include "UnableThroughEdge.h"
#include "UnableThroughBlock.h"
#include "CircularSaw.h"
#include "DrawMode.h"
#include "Input.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "ItemEmitter.h"
#include "ScoreManager.h"
#include "IActionState.h"
#include "Easing.h"

ComPtr<ID3D12Resource> BaseGameActor::constCameraBuff = nullptr;
DebugCamera* BaseGameActor::camera = nullptr;
bool BaseGameActor::rotCamera = false;
float BaseGameActor::radY = 0;
int BaseGameActor::cameraRotCount = 0;
const int BaseGameActor::ROT_TIME = 10;

BaseGameActor::BaseGameActor(const Vector3& arg_pos)
	:START_POS(arg_pos),
	 RADIUS(1.0f),
	 weight(10),
	 prePos(arg_pos),
	 virtualityPlanePosition(arg_pos),
	 preVirtualityPlanePosition(arg_pos),
	 direction(Vector3(0, 0, 1)),
	 isCrushed(false),
	 isEndGame(false),
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
	 fallEasingCount(0),
	 fallStartPos(Vector3()),
	 fallEndPos(Vector3()),
	 isPlayedFallSound(false)
{
	position = arg_pos;
	pObjectManager = ObjectManager::GetInstance();

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, XMFLOAT4(1, 1, 0, 0.6f));
	panelCutLocus->SetParentObject(this);

	Initialize();
}

BaseGameActor::~BaseGameActor()
{
	delete panelCountSprite3D;
}

void BaseGameActor::Initialize()
{
	scale = { 0.9f };
	rotation = 0;
	position = START_POS;
	weight = 10;
	prePos = position;
	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	direction = Vector3(0, 0, 1);
	panelCountSprite3D->Initialize();
	isCrushed = false;
	isEndGame = false;
	//actionState
	speed = WALK_SPEED;
	isHitDuringTackle = false;
	tackleStartPos = Vector3();
	tackleEndPos = Vector3();
	tackleCount = 0;
	blownTime = 0;
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
	fallEasingCount = 0;
	fallStartPos = Vector3();
	fallEndPos = Vector3();
	isPlayedFallSound = false;
}

void BaseGameActor::Update()
{
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
		Field* field = ActorManager::GetInstance()->GetFields()[0];
		CuttingInfo* info = field->GetCuttingInfo(this);

		//actionState = actionState->Update(this);

		field->DecideCuttingInfo(this, virtualityPlanePosition, direction);
		if (/*!drawingFlag*/true)
		{
			panelCutLocus->SetCutPower(cutPower);
			panelCutLocus->Move(info->cuttingStartPos, info->cuttingAngle);
		}
	}

	Object::Update();
	panelCountSprite3D->Update();
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

	if (Object3D::GetDrawShadow())
		pipelineName = "FBXShadowMap";
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

void BaseGameActor::StayInTheField()
{
	//Withstandは不実行
	/*if (standingFlag || returningFieldFlag)
	{
		return;
	}*/

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
			//StartStand();
			SuspendTackle();
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			//StartStand();
			SuspendTackle();
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
			//StartStand();
			SuspendTackle();
			break;
		}
	}

	//Withstandになっていたらreturn
	/*if (!standingFlag)
	{
		return;
	}*/

	//CutならCutを中断
	/*if (drawingFlag)
	{
		HitOnDrawing();
	}*/
}

void BaseGameActor::StayOnRemainPanels()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, RADIUS);

	if (piece)
	{
		virtualityPlanePosition = preVirtualityPlanePosition;

		Vector3 outPieceVec = {};
		outPieceVec = position - piece->GetVirtualityPlanePosition();
		outPieceVec.Normalize();
		StartWithstand(false, outPieceVec);

		//Tackle中なら
		if (/*tackleFlag*/true)
		{
			SuspendTackle();
		}
		return;
	}

	//即落ち
	//Withstandなら
	if (/*!standingFlag*/false)
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
				//fallFlag = true;
				//Fallになる
				fallStartPos = virtualityPlanePosition;
				fallEndPos = p->GetVirtualityPlanePosition();
				return;
			}
		}
	}
}

void BaseGameActor::DischargeGottenPanel(BaseGameActor* arg_actor)
{
	if (/*arg_actor->IsFall()*/false)
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
		//Audio::PlayWave("SE_TriangleLost");
	}
}

void BaseGameActor::UpdatePos()
{
}

void BaseGameActor::Move()
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	//走りと歩きの切り替え処理
	if (/*drawingFlag*/false)
	{
		speed = DRAWING_SPEED;
	}
	else
	{
		speed = WALK_SPEED;
	}

	if (/*standingFlag*/false)
	{
		Withstand();
	}
	else
	{
		SlidingDown();
	}

	//フィールド端からの復帰
	if (/*standingFlag || returningFieldFlag*/false)
	{
		//myModel->PlayAnimation("stand", true);
		if (isReturningField)
		{
			virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
			moveEasingCount++;
			if (moveEasingCount >= 30)
			{
				moveEasingCount = 0;
				isReturningField = false;
			}
		}
	}
	//タックルの移動　ガチで月曜にplayerリファクタリングする
	else if (/*tackleFlag*/false)
	{
		virtualityPlanePosition = EasingMove(tackleStartPos, tackleEndPos, 1, tackleCount / 30.0f);
		tackleCount++;
		if (tackleCount >= 30)
		{
			tackleCount = 0;
			//tackleFlag = false;
			isHitDuringTackle = false;
		}
		StayInTheField();
		StayOnRemainPanels();
	}
	//通常の移動
	else
	{
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
			StayInTheField();
			StayOnRemainPanels();
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

		if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
		{
			//myModel->PlayAnimation("stand", true);
		}
		else if (/*drawingFlag*/false)
		{
			//myModel->PlayAnimation("run", true);
		}
		else
		{
			//myModel->PlayAnimation("walk", true);
		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void BaseGameActor::SuspendTackle()
{
	if (/*standingFlag || returningFieldFlag*/true)
	{
		//tackleFlag = false;
		tackleCount = 0;
		speed = WALK_SPEED;
	}
	//タックル中
	else
	{
		tackleEndPos = virtualityPlanePosition;
		//tackleCount = 0;
		speed = WALK_SPEED;
	}
}

void BaseGameActor::StartWithstand(bool arg_outField, Vector3 arg_velocity)
{
	//standingFlag = true;
	//タックル終了
	SuspendTackle();

	inputStartCount = nextInputStartCount;
	withstandTime = 120;

	//場外か図形か
	if (arg_outField)
	{
		preWithstandVec = -position; //中央に向かう
	}
	else
	{
		preWithstandVec = arg_velocity; //パネルの中心からプレイヤーに向かうベクトル
	}

	preWithstandVec.y = 0;
	preWithstandVec.Normalize();

	Audio::PlayWave("SE_SteppingOn", 0.25f, true);
}

void BaseGameActor::EndDrawing()
{
	//drawingFlag = false;
	panelCutLocus->RecordCuttedPanelPos();
	int num = ActorManager::GetInstance()->GetFields()[0]->CutPanel(panelCutLocus);
	weight += num * FieldPiece::GetWeight();
	gottenPanel += num;
	cutPower = 0;

	ScoreManager::GetInstance()->AddScore_CutPanel(num);

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);
	virtualityPlanePosition = info->ridingPiece->GetVirtualityPlanePosition();
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void BaseGameActor::HitOnDrawing()
{
	auto c = ActorManager::GetInstance()->GetCircularSaw(this);
	if (c)
	{
		c->Dead();
	}
	//drawingFlag = false;
}

void BaseGameActor::HitCheckActor(BaseGameActor* arg_actor)
{
	/*if (blowFlag || isHitDuringTackle)
	{
		return;
	}*/	

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(arg_actor->GetVirtualityPlanePosition() - virtualityPlanePosition));
	if (length <= RADIUS + arg_actor->RADIUS)
	{
		HitActor(arg_actor);
	}
}

void BaseGameActor::HitActor(BaseGameActor* arg_actor)
{
	static const float weightCoefficient = 0.3f;

	//相手がFallならreturn
	/*if (arg_actor->IsFall())
	{
		return;
	}*/

	Audio::PlayWave("SE_Collision", 1.0f);


	//汎用化	
	//相手を落とす
	if (/*arg_actor->GetStanding() && tackleFlag*/false)
	{
		//arg_actor->StartFall();
	}
	//自分が落とされる
	else if (/*standingFlag && arg_actor->IsTackle()*/false)
	{
		Object::SetColor({ 1,1,1,1 });
		/*standingFlag = false;
		fallFlag = true;*/
		fallStartPos = virtualityPlanePosition;
		fallEndPos = virtualityPlanePosition + (-preWithstandVec * 4);

		Audio::StopWave("SE_SteppingOn");
	}
	//ただの衝突
	else
	{
		/*arg_actor->StartBlow();
		arg_actor->SetBlownTime(40);*/
	}



	/*blowFlag = true;
	blowTime = 40;*/


	Vector3 enemyPos = arg_actor->GetVirtualityPlanePosition();
	Vector3 enemyVel = arg_actor->GetVelocity();
	float enemyWeight = arg_actor->GetWeight() * weightCoefficient;

	float playerWeight = weight;

	/*if (tackleFlag)
	{
		playerWeight += 20;
	}*/

	float totalWeight = (playerWeight * weightCoefficient) + enemyWeight;
	float refRate = (1 + 1 * 1); //反発率をプレイヤー、エネミーそれぞれ持たせる
	Vector3 c = virtualityPlanePosition - enemyPos;
	c.Normalize();

	float dot = Vector3::Dot((velocity - enemyVel), c);
	Vector3 constVec = refRate * dot / totalWeight * c;

	//衝突後速度ベクトル
	Vector3  playerAfterVel = -enemyWeight * constVec + velocity;
	Vector3  enemyAfterVel = (playerWeight * weightCoefficient) * constVec + enemyVel;

	//どちらもFallでなかったら
	if (/*!fallFlag && !arg_actor->IsFall()*/true)
	{
		DischargeGottenPanel(arg_actor);
		arg_actor->DischargeGottenPanel(this);
	}

	if (/*tackleFlag*/false)
	{
		//タックルで敵だけ飛ばす
		//blowFlag = false;
		blownTime = 0;
		isHitDuringTackle = true;
		SuspendTackle();
		arg_actor->SetVelocity(enemyAfterVel.Normalize());

		//パーティクル
		ParticleEmitter::ShockEffect((arg_actor->GetPosition() + position) / 2.0f, Vector3(255.0f, 255.0f, 255.0f));
	}
	else
	{
		velocity = playerAfterVel.Normalize();
		arg_actor->SetVelocity(enemyAfterVel.Normalize());
	}

	if (/*drawingFlag*/true)
	{
		HitOnDrawing();
	}
	if (/*arg_actor->IsDrawing()*/true)
	{
		arg_actor->HitOnDrawing();
	}

	////衝突後位置
	//Vector3 playerAfterPos = position + blowTime * playerAfterVel;
	//Vector3 enemyAfterPos = enemyPos + standardEnemy->GetBlowTime() * enemyAfterVel;
}

void BaseGameActor::HitCheckEnergyItem(EnergyItem* arg_energyItem)
{
	if (!arg_energyItem->IsAppeared())
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
	switch (arg_energyItem->GetRank())
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
	}

	if (cutPower > 6)
	{
		cutPower = 6;
	}

	Audio::PlayWave("SE_GetTriangle");
	arg_energyItem->Dead();
}

void BaseGameActor::HitCheckPanelItem(PanelItem* arg_panelItem)
{
	if (!arg_panelItem->IsEndBounce())
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
	/*if (drawingFlag)
	{
		return;
	}*/

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.05f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
	StayOnRemainPanels();

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
	//ふっとばされ中
	if (/*blowFlag*/false)
	{
		//velocityに入っている値に進むように
		arg_direction = velocity;
		speed = BLOWN_SPEED;
		return;
	}

	if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
	{
		return;
	}

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//くり抜き動作中
	if (/*drawingFlag*/false)
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

void BaseGameActor::Tackle()
{
	//Moveからのみ実行可能
	/*if (tackleFlag || blowFlag || drawingFlag || standingFlag || returningFieldFlag)
	{
		return;
	}*/
	//tackleFlag = true;

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
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;

	Audio::PlayWave("SE_Dash");
}

void BaseGameActor::Withstand()
{
	//Cytならreturn
	if (/*drawingFlag*/false)
	{
		return;
	}

	//踏ん張り中　赤
	Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	if (BGColor >= 0)
	{
		BGColor -= 0.02f;
	}
	else
	{
		BGColor = 1;
	}

	Object::SetColor({ 1,1,1,1 });
	//Fallに
	/*standingFlag = false;
	fallFlag = true;*/
	fallStartPos = virtualityPlanePosition;
	fallEndPos = virtualityPlanePosition + (-preWithstandVec * 4);

	Audio::StopWave("SE_SteppingOn");


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
}

void BaseGameActor::Fall()
{
	if (fallEasingCount <= 30)
	{
		fallEasingCount++;
		virtualityPlanePosition = EasingMove(fallStartPos, fallEndPos, 1, fallEasingCount / 30.0f);
	}
	else
	{
		virtualityPlanePosition.y -= 2;
	}


	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}
