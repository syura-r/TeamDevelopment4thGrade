#include "Player.h"

#include"OBJLoader.h"
#include"SphereCollider.h"
#include"Input.h"
#include"DebugText.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "GameSettingParam.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "TestBoss.h"
#include "BossMissile.h"
#include "BossRangeAttack.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"

DebugCamera* Player::camera = nullptr;

Player::Player()
{
	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("player");
	//���f���̐���
	Create(myModel);
	//�����蔻��(Box)�̐���
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale({0.2f,0.5f,0.2f});
	boxCollider->SetOffset({ 0,0.5f,0 ,0 });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	collider->SetMove(true);

	pObjectManager = ObjectManager::GetInstance();

	//locusSelecter = new LocusSelecter();

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);	
	attackSprite = new Sprite();

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("Player", this);

	Initialize();

	//�萔�o�b�t�@�̍쐬
	HRESULT result;
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constCameraBuff));
	assert(SUCCEEDED(result));


	//�f�v�X�V���h�E�p�̃J��������ݒ�
	ConstLightCameraBuff* constMap2 = nullptr;
	result = constCameraBuff->Map(0, nullptr, (void**)&constMap2);
	assert(SUCCEEDED(result));
	constMap2->cameraPos = Object3D::GetLightCamera()->GetEye();
	constMap2->viewProjection = XMMatrixLookAtLH(Vector3(Object3D::GetLightCamera()->GetEye()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetTarget()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetUp()).ConvertXMVECTOR()) * XMMatrixOrthographicLH(100, 100, 1.0f, 1000.0f);
	constCameraBuff->Unmap(0, nullptr);

	
}

Player::~Player()
{	
	delete attackSprite;	
	//delete locusSelecter;		
	ActorManager::GetInstance()->DeleteObject(this);
}

void Player::Initialize()
{
	onGround = true;
	scale = { 0.9f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };
	cameraRotCount = 0;
	rotCamera = false;
	radY = 0;
	camera->SetTarget(position + Vector3{ 0, 1, 0 });

	camera->Update();
	//----------�J�����̉�]�p�x���Z�o---------------
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
	//�J�����̉�]
	//camera->AddPhi(rad);
	camera->SetTheta(120);
	

	camera->SetDistance(100);

	drawingFlag = false;
	isExtendLine = false;
	currentLineNum = 0;	
	DeleteDrawingLine();
	for (int i = 0; i < vecLocuss.size(); i++)
	{
		delete vecLocuss[i];
		vecLocuss[i] = nullptr;
	}
	vecLocuss.clear();
	/*nowDrawingLocus = predictStar;
	predictStar->ChangeIsDraw(true);*/	
	feverQuota = maxFeverQuota;		
	/*locusSelecter->Initialize();
	locusSelecter->Setting();*/	
	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	weight = 10;
}

void Player::Update()
{
#ifdef _DEBUG
	//�p�[�e�B�N���m�F�p
	if (Input::TriggerKey(DIK_RETURN))
	{
		ParticleEmitter::CreateExplosion(position);
		ParticleEmitter::CreateAir(position);
		ParticleEmitter::CreateGetEffect(position);

	}
#endif
	
	//locusSelecter->Update();

	//�ړ�����
	Move();

	//�J�����̃��Z�b�g����
	MoveCamera();

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	if (blowFlag)
	{
		blowTime--;
		if (blowTime <= 0)
		{
			blowFlag = false;
		}
	}
	else
	{
		//SelectLocus();		
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) && panelCutLocus->GetCutPower() > 0 && field->GetPlayerRidingPiece())
		{
			if (!tackleFlag)
			{
				drawingFlag = true;
				//���̐���
				//CreateLine();
				Vector3 p = field->GetPlayerCuttingStartPos();
				ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus));
			}
			
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
		{	
			Tackle();
		}

		//�h���[�C���O�̏���
		DrawingLine();
		
	}

	//�A�C�e������(��)
	if (Input::TriggerKey(DIK_B))
	{
		EmitEnergyItem();
	}
	
	//�}�`�̏���
	if (!drawingFlag)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::TriggerPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			if (!vecLocuss.empty())
			{				
				DeleteLocuss();
			}
		}
	}
	
	//�����蔻��n
	HitCheckLoci();	
	HitCheckEnemy();
	HitCheckItems();	
	
	//���̃I�u�W�F�N�g�Ƃ̃q�b�g�`�F�b�N
	//CheckHit();
	Object::Update();	
	
	Vector3 p = field->GetPlayerCuttingStartPos();
	//SetLocus(LocusType::UNDIFINED);
	if (!drawingFlag)
	{		
		panelCutLocus->SetCutPower(2);
		panelCutLocus->Move(p, LocusUtility::Vector3XZToAngle(direction));
	}
	for (auto locus : vecLocuss)
	{
		locus->Move(locus->GetVirtualityPlanePosition(), locus->GetAngle());
	}
}

void Player::Draw()
{
	
	if (!Object3D::GetDrawShadow())
	{
		HRESULT result;

		//�萔�o�b�t�@���Z�b�g
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constCameraBuff->GetGPUVirtualAddress());
	}
	CustomDraw(true, true);
	if (!Object3D::GetDrawShadow())
	{
		if (!vecLocuss.empty())
		{
			attackSprite->DrawSprite("s_LBorRB", Vector2(960, 150), 0.0f, Vector2(1.5f, 1.5f));
		}
		
		//locusSelecter->Draw();		
	}
}

void Player::DrawReady()
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
		ImGui::Text("inputAccuracy : {%f}\n", inputAccuracy);
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

void Player::Move()
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	//����ƕ����̐؂�ւ�����
	if (drawingFlag)
	{
		speed = drawingSpeed;
	}
	else
	{
		speed = walkSpeed;
	}

	if (standingFlag)
	{
		WithStand();
	}
	else
	{
		SlidingDown();
	}

	//�t�B�[���h�[����̕��A
	if (standingFlag || returningFieldFlag)
	{
		myModel->PlayAnimation("stand", true);
		if (returningFieldFlag)
		{
			virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
			moveEasingCount++;
			if (moveEasingCount >= 30)
			{
				moveEasingCount = 0;
				returningFieldFlag = false;
			}
		}
	}
	//�^�b�N���̈ړ��@�K�`�Ō��j��player���t�@�N�^�����O����
	else if (tackleFlag)
	{
		virtualityPlanePosition = EasingMove(tackleStartPos, tackleEndPos, 1, tackleCount / 30.0f);
		tackleCount++;
		if (tackleCount >= 30)
		{
			tackleCount = 0;
			tackleFlag = false;
			tackleHitFlag = false;
		}
		StayInTheField();
	
	}
	//�ʏ�̈ړ�
	else
	{
		//�ړ�����
		Vector3 moveDirection = direction;
		DecideDirection(moveDirection);

		//��]����
		//���݂̐i�s�����ƃJ�����̐��ʂƊp�x�����߂�
		direction.Normalize();
		float cosA = direction.Dot(moveDirection);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;

		float rotY = acos(cosA) * 180 / 3.14159365f;
		const Vector3 CrossVec = direction.Cross(moveDirection);

		//���͂��Ȃ��̂�0�ɂ���
		if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
		{
			moveDirection = Vector3(0, 0, 0);
		}

		float rotSpeed = rotateSpeed;
		if (abs(rotY) < 55)
		{
			virtualityPlanePosition += moveDirection * (speed * inputAccuracy);
			StayInTheField();
			isExtendLine = true;
		}
		else
		{
			isExtendLine = false;
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
			myModel->PlayAnimation("stand", true);
		}
		else if (drawingFlag)
		{
			myModel->PlayAnimation("run", true);
		}
		else
		{
			myModel->PlayAnimation("walk", true);
		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}


void Player::MoveCamera()
{
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();

	//�J�����̃��Z�b�g����
	if (Input::TriggerKey(DIK_C) && !rotCamera)
	{
		rotCamera = true;
		float cosA = direction.Dot(cameraDirectionZ);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;
		radY = acos(cosA);
		const Vector3 CrossVec = direction.Cross(cameraDirectionZ);
		if (CrossVec.y < 0)
			radY *= -1;
		cameraRotCount = 0;
		//camera->AddPhi(radY);
	}
	
	//�J�����̉�]����
	if (rotCamera)
	{
		cameraRotCount++;
		float rad = radY / RotTime;
		camera->AddPhi(rad);
		if (cameraRotCount >= RotTime)
			rotCamera = false;
	}
}

void Player::SlidingDown()
{
	if (drawingFlag)
	{
		return;
	}	

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.05f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
}

void Player::DecideDirection(Vector3& arg_direction)
{
	//�ӂ��Ƃ΂��ꒆ
	if (blowFlag)
	{
		//velocity�ɓ����Ă���l�ɐi�ނ悤��
		arg_direction = velocity;
		inputAccuracy = 1;
		speed = blowSpeed;
		return;
	}

	if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
	{
		return;
	}

	//�J�����̃r���[�s��̋t�s����v�Z
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//���蔲�����쒆
	if (drawingFlag)
	{
		arg_direction = { 0,0,0 };
		//if (Input::CheckPadLStickAnythingDir())
		//{
		//	arg_direction = nowDrawingLocus->GetLine(currentLineNum)->GetDirection();

		//	//�X�e�B�b�N�̌���
		//	Vector2 stickDirection = Vector2(0, 0);
		//	auto vec = Input::GetLStickDirection();
		//	stickDirection.x = (cameraDirectionX * vec.x).x;
		//	stickDirection.y = (cameraDirectionZ * vec.y).z;
		//	stickDirection = Vector2::Normalize(stickDirection);

		//	//���̌���
		//	auto lineVec = nowDrawingLocus->GetLine(currentLineNum)->GetDirection();

		//	inputAccuracy = Vector2::Dot(stickDirection, LocusUtility::Dim3ToDim2XZ(lineVec));

		//	if (inputAccuracy <= 0)
		//	{
		//		inputAccuracy = 0;
		//	}

		//	inputAccuracy = Easing::EaseOutCirc(0, 1, 1, inputAccuracy);
		//}
		//else
		//{
		//	inputAccuracy = 0; //�X�e�B�b�N���͂��Ȃ����瓮���Ȃ�
		//}
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
		inputAccuracy = 1;
	}

	arg_direction.Normalize();
	//�����p�ɑ��
	velocity = arg_direction;
}

//void Player::SelectLocus()
//{
//	if (drawingFlag)
//	{
//		return;
//	}
//
//	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
//	{
//		pressedButton = LocusSelecter::Button::BBUTTON;
//	}
//	else if (Input::TriggerPadButton(XINPUT_GAMEPAD_X))
//	{
//		pressedButton = LocusSelecter::Button::XBUTTON;
//	}
//	else if (Input::TriggerPadButton(XINPUT_GAMEPAD_Y))
//	{
//		pressedButton = LocusSelecter::Button::YBUTTON;
//	}
//
//	switch (pressedButton)
//	{
//	case LocusSelecter::UNDIFINED:
//		break;
//	case LocusSelecter::XBUTTON:
//		SetLocus(locusSelecter->XbuttonLocusType());
//		break;
//	case LocusSelecter::YBUTTON:
//		SetLocus(locusSelecter->YbuttonLocusType());
//		break;
//	case LocusSelecter::BBUTTON:
//		SetLocus(locusSelecter->BbuttonLocusType());
//		break;
//	default:
//		break;
//	}
//}

//void Player::SetLocus(LocusType arg_LocusType)
//{
//	if (nowDrawingLocus)
//	{
//		nowDrawingLocus->ChangeIsDraw(false);
//	}
//
//	switch (arg_LocusType)
//	{
//	case LocusType::UNDIFINED:
//		nowDrawingLocus = nullptr;
//		break;
//	case LocusType::TRIANGLE:
//		nowDrawingLocus = predictTriangle;
//		break;
//	case LocusType::RIBBON:
//		nowDrawingLocus = predictRibbon;
//		break;
//	case LocusType::PENTAGON:
//		nowDrawingLocus = predictPentagon;
//		break;
//	case LocusType::STAR:
//		nowDrawingLocus = predictStar;
//		break;
//	case LocusType::HEXAGRAM:
//		nowDrawingLocus = predictHexagram;
//		break;
//	case LocusType::TRIFORCE:
//		nowDrawingLocus = predictTriforce;
//		break;
//	default:
//		break;
//	}
//
//	if (arg_LocusType != LocusType::UNDIFINED)
//	{
//		nowDrawingLocus->ChangeIsDraw(true);
//	}
//}

void Player::CreateLine()
{
	if (currentLineNum >= panelCutLocus->GetMaxNumLine())
	{
		return;
	}

	Vector3 nowLineVel = panelCutLocus->GetLine(currentLineNum)->GetDirection(); //kokokokoko
	pNowDrawingLine = new Line(virtualityPlanePosition, LocusUtility::Vector3XZToAngle(nowLineVel), 0, { 1,1,1,1 }, Vector3(0.5f, 0.7f, 0.7f));
	ObjectManager::GetInstance()->Add(pNowDrawingLine, true);
	vecDrawingLines.push_back(pNowDrawingLine);
}

void Player::DrawingLine()
{
	if (pNowDrawingLine != nullptr)
	{
		//�{�^���������Ă��邩�h���[�C���O���͐���L�΂�
		if (Input::CheckPadButton(XINPUT_GAMEPAD_A) && drawingFlag)
		{	
			if (isExtendLine)
			{				
				if (Input::DownWASD() || Input::CheckPadLStickAnythingDir())
				{
					pNowDrawingLine->AddLength(speed * inputAccuracy);
				}
			}			

			float lengthNowLine = pNowDrawingLine->GetLength();
			float lengthLocusLine = panelCutLocus->GetLine(currentLineNum)->GetLength();

			if (lengthLocusLine - lengthNowLine <= 0.05f) //�}�W�b�N�T�C�R�[
			{
				pNowDrawingLine->SetLength(lengthLocusLine);
				currentLineNum++;
				if (currentLineNum >= panelCutLocus->GetMaxNumLine())
				{
					drawingFlag = false;
					currentLineNum = 0;
					//static const XMFLOAT4 copyColor = XMFLOAT4(0.1f, 0.3f, 0.9f, 0.6f);
					////�����Ő}�`�Ƃ��ĕۑ����鏈��
					//BaseLocus* copyLocus = nullptr;
					//switch (nowDrawingLocus->GetType())
					//{
					//case LocusType::UNDIFINED:
					//	break;
					//case LocusType::TRIANGLE:
					//	copyLocus = new TestTriangle(*predictTriangle, copyColor);
					//	break;
					//case LocusType::RIBBON:
					//	copyLocus = new TestRibbon(*predictRibbon, copyColor);
					//	break;
					//case LocusType::PENTAGON:
					//	copyLocus = new TestPentagon(*predictPentagon, copyColor);
					//	break;
					//case LocusType::STAR:
					//	copyLocus = new TestStar(*predictStar, copyColor);
					//	break;
					//case LocusType::HEXAGRAM:
					//	copyLocus = new TestHexagram(*predictHexagram, copyColor);
					//	break;
					//case LocusType::TRIFORCE:
					//	copyLocus = new TestTriforce(*predictTriforce, copyColor);
					//	break;
					//default:
					//	break;
					//}

					//Field* field = ActorManager::GetInstance()->GetFields()[0];
					//if (field)
					//{
					//	field->AddInfluence(LocusFieldInfluence{ copyLocus->GetCenterOfGravity(), copyLocus->GetWeight() });
					//}
					weight += 8;
					// 
					//vecLocuss.push_back(copyLocus);
					
					//MoveEndDrawing(copyLocus);
					DeleteDrawingLine();
					//locusSelecter->SetNextLocus(pressedButton);
					return;
				}
				position = panelCutLocus->GetLine(currentLineNum)->GetStartPos();
				CreateLine();
			}
		}
		else
		{
			SuspendDrawing();
		}
	}

}

void Player::DeleteDrawingLine()
{
	for (int i = 0; i < vecDrawingLines.size(); i++)
	{
		vecDrawingLines[i]->Dead();
	}
	vecDrawingLines.clear();
}

void Player::SuspendDrawing()
{
	drawingFlag = false;
	currentLineNum = 0;
	DeleteDrawingLine();
	pNowDrawingLine = nullptr;
}

void Player::DeleteLocuss()
{
	auto end = vecLocuss.size();	

	for (int i = 0; i < end; i++)
	{
		delete vecLocuss[i];
		vecLocuss[i] = nullptr;
	}
	vecLocuss.clear();	

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	if (field)
	{
		field->ResetInfluences();
	}
	weight = 5;
}

void Player::MoveEndDrawing(BaseLocus* arg_locus)
{
	Vector3 vec = LocusUtility::AngleToVector2(arg_locus->GetAngle() + 180);
	virtualityPlanePosition = arg_locus->GetLine(arg_locus->GetMaxNumLine() - 1)->GetVirtualityPlaneEndPos();
	virtualityPlanePosition += vec * 2.0f;
	StayInTheField();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void Player::StayInTheField()
{
	if (standingFlag || returningFieldFlag)
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

		//���������Ă��邩
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > RADIUS)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			IsStand();
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			IsStand();
			break;
		}

		//�ʂ�߂�����
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			IsStand();
			break;
		}
	}

	if (!standingFlag)
	{
		return;
	}

	if (drawingFlag)
	{
		SuspendDrawing();
	}
}

void Player::HitCheckEnemy()
{
	if (blowFlag || tackleHitFlag)
	{
		return;
	}

	std::vector<StandardEnemy*> enemies = ActorManager::GetInstance()->GetStandardEnemies();	

	for (auto itr = enemies.begin(); itr != enemies.end(); itr++)
	{
		float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ((*itr)->GetVirtualityPlanePosition() - virtualityPlanePosition));
		if (length <= 2.0f)
		{
			HitEnemy(*itr);
		}
	}
}

void Player::HitEnemy(StandardEnemy* arg_enemy)
{
	static const float weightCoefficient = 0.3f;
	//�ėp��	
	arg_enemy->IsBlow();
	blowFlag = true;
	
	blowTime = 40;
	arg_enemy->SetBlowTime(40);

	Vector3 enemyPos = arg_enemy->GetVirtualityPlanePosition();
	Vector3 enemyVel = arg_enemy->GetVelocity();
	float enemyWeight = arg_enemy->GetWeight() * weightCoefficient;

	float playerWeight = weight;

	if (tackleFlag)
	{
		playerWeight += 20;
	}

	float totalWeight = (playerWeight * weightCoefficient) + enemyWeight;
	float refRate = (1 + 1 * 1); //���������v���C���[�A�G�l�~�[���ꂼ�ꎝ������
	Vector3 c = virtualityPlanePosition - enemyPos;
	c.Normalize();

	float dot = Vector3::Dot((velocity - enemyVel), c);
	Vector3 constVec = refRate * dot / totalWeight * c;

	//�Փˌ㑬�x�x�N�g��
	Vector3  playerAfterVel = -enemyWeight * constVec + velocity;
	Vector3  enemyAfterVel = (playerWeight * weightCoefficient) * constVec + enemyVel;

	if (tackleFlag)
	{
		//�^�b�N���œG������΂�
		blowFlag = false;
		blowTime = 0;
		tackleHitFlag = true;
		SuspendTackle();
		arg_enemy->SetVelocity(enemyAfterVel.Normalize());
	}
	else
	{
		velocity = playerAfterVel.Normalize();
		arg_enemy->SetVelocity(enemyAfterVel.Normalize());
	}
	

	SuspendDrawing();

	////�Փˌ�ʒu
	//Vector3 playerAfterPos = position + blowTime * playerAfterVel;
	//Vector3 enemyAfterPos = enemyPos + standardEnemy->GetBlowTime() * enemyAfterVel;
}

void Player::HitCheckItems()
{
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();

	for (auto item : items)
	{
		if (false)
		{
			HitItem(item);
		}
	}
}

void Player::HitItem(EnergyItem* arg_item)
{
}

void Player::IsStand()
{
	standingFlag = true;
	//�^�b�N���I��
	SuspendTackle();

	standTime = 120;
	preStandVec = -position;
	preStandVec.y = 0;
	preStandVec.Normalize();
}

void Player::WithStand()
{
	if (drawingFlag)
	{
		return;
	}	

	//���񒣂蒆�@��
	Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	if (BGColor >= 0)
	{
		BGColor -= 0.02f;
	}
	else
	{
		BGColor = 1;
	}

	//�J�����̃r���[�s��̋t�s����v�Z
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	//�X�e�B�b�N�̌���
	auto vec = Input::GetLStickDirection();
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	float accuracy = 0;

	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	accuracy = Vector2::Dot(stickDirection, correctVec);

	if (accuracy <= 0)
	{
		accuracy = 0;
	}

	if (accuracy >= 0.7f)
	{
		Vector3 moveDirection = preStandVec;	
		Object::SetColor({ 1,1,1,1 });
		returningFieldFlag = true;
		standingFlag = false;
		returningStartPos = virtualityPlanePosition;
		returningEndPos = virtualityPlanePosition + moveDirection * 3;
		return;
	}

	/*standTime--;
	if (standTime <= 0)
	{
		Object::SetColor({ 1,1,1,1 });
		isStanding = false;
	}*/
}

void Player::Tackle()
{
	if (tackleFlag || blowFlag || drawingFlag || standingFlag || returningFieldFlag )
	{
		return;
	}

	tackleFlag = true;

	//�J�����̃r���[�s��̋t�s����v�Z
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//�X�e�B�b�N�̌���
	auto vec = Input::GetLStickDirection();
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;


}

void Player::SuspendTackle()
{
	if (standingFlag || returningFieldFlag)
	{
		tackleFlag = false;
		tackleCount = 0;
		speed = walkSpeed;
	}
	//�^�b�N����
	else
	{
		tackleEndPos = virtualityPlanePosition;
		//tackleCount = 0;
		speed = walkSpeed;
	}
	
}

Vector3 Player::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	Vector3 result = {};
	result.x = Easing::EaseOutCubic(arg_startPos.x, arg_endPos.x, arg_maxTime, arg_nowTime);
	result.y = Easing::EaseOutCubic(arg_startPos.y, arg_endPos.y, arg_maxTime, arg_nowTime);
	result.z = Easing::EaseOutCubic(arg_startPos.z, arg_endPos.z, arg_maxTime, arg_nowTime);
	return result;
}

void Player::EmitEnergyItem()
{
	EnergyItem* item = new EnergyItem(virtualityPlanePosition + Vector3(0, EnergyItem::GetRadius(), 0));
	pObjectManager->Add(item);
}

bool Player::IsAlive()
{
	return true;
}

void Player::EndDrawing()
{
	drawingFlag = false;
}

Vector3 Player::GetDirection() const
{
	return direction;
}

void Player::HitCheckLoci()
{
	if (virtualityPlanePosition == preVirtualityPlanePosition)
	{
		return;
	}

	for (auto locus : vecLocuss)
	{
		for (int i = 0; i < locus->GetMaxNumLine(); i++)
		{
			Line* line = locus->GetLine(i);
			Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneStartPos());
			Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneEndPos());
			Vector2 AB = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos() - line->GetVirtualityPlaneStartPos());
			Vector2 normalAB = Vector2::Normalize(AB);

			//���������Ă��邩
			float cross = Vector2::Cross(AO, normalAB);
			if (fabsf(cross) > RADIUS)
			{
				continue;
			}

			float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
			if (multiDot <= 0.0f)
			{
				HitLoci(line);
				continue;
			}

			if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
			{
				HitLoci(line);
				continue;
			}

			//�ʂ�߂�����
			Vector2 start = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneStartPos());
			Vector2 end = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos());
			Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
			Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

			if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
				LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
			{
				HitLoci(line);
			}
		}
	}
}

void Player::HitLoci(Line* arg_line)
{
	position = prePos;
	virtualityPlanePosition = preVirtualityPlanePosition;

	if (drawingFlag)
	{
		SuspendDrawing();
	}
}
