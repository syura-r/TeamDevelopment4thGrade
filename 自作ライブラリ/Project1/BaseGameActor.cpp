#include "BaseGameActor.h"
#include "Field.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "PanelCutLocus.h"
#include "UnableThroughEdge.h"
#include "UnableThroughBlock.h"
#include "DrawMode.h"
#include "Input.h"
#include "ActorManager.h"
#include "IState.h"

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
}

void BaseGameActor::StayOnRemainPanels()
{
}

void BaseGameActor::DischargeGottenPanel(BaseGameActor* arg_actor)
{
}

void BaseGameActor::UpdatePos()
{
}

void BaseGameActor::Move()
{
}

void BaseGameActor::SuspendTackle()
{
}

void BaseGameActor::StartWithstand(bool arg_outField, Vector3 arg_velocity)
{
}

void BaseGameActor::EndDrawing()
{
}

void BaseGameActor::HitOnDrawing()
{
}

void BaseGameActor::HitCheckActor(BaseGameActor* arg_actor)
{
}

void BaseGameActor::HitActor(BaseGameActor* arg_actor)
{
}

void BaseGameActor::HitCheckEnergyItem(EnergyItem* arg_energyItem)
{
}

void BaseGameActor::HitEnergyItem(EnergyItem* arg_energyItem)
{
}

void BaseGameActor::HitCheckPanelItem(PanelItem* arg_panelItem)
{
}

void BaseGameActor::HitPanelItem(PanelItem* arg_panelItem)
{
}

void BaseGameActor::HitCheckUnableThroughEdge(UnableThroughEdge* arg_edge)
{
}

void BaseGameActor::HitUnableThroughEdge(UnableThroughEdge* arg_edge)
{
}

void BaseGameActor::HitCheckUnableThroughBlock(UnableThroughBlock* arg_block)
{
}

void BaseGameActor::HitUnableThroughBlock(UnableThroughBlock* arg_block)
{
}

void BaseGameActor::SlidingDown()
{
}

Vector3 BaseGameActor::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	return Vector3();
}

void BaseGameActor::DecideDirection(Vector3& arg_direction)
{
}

void BaseGameActor::Tackle()
{
}

void BaseGameActor::Withstand()
{
}

void BaseGameActor::Fall()
{
}
