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

	locusSelecter = new LocusSelecter();

	predictStar = new TestStar(Vector3(0, -5, 0), 90);
	predictStar->ChangeIsDraw(false);
	predictTriforce = new TestTriforce(Vector3(0, -5, 0), 90);
	predictTriforce->ChangeIsDraw(false);
	predictRibbon = new TestRibbon(Vector3(0, -5, 0), 90);
	predictRibbon->ChangeIsDraw(false);
	inFeverTimer = new Timer(300);
	measurer = new NormalWaveMeasurer();

	Initialize();

	//�萔�o�b�t�@�̍쐬
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffData) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));

	}
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constCameraBuff));
	assert(SUCCEEDED(result));

	//�|���S�����������̃X�e�[�^�X��������
	sendData._Destruction = 1;
	sendData._PositionFactor = 2;
	sendData._RotationFactor = 0.2f;
	sendData._ScaleFactor = 1;
	sendData._Tessellation = 1;
	sendData._OnEasing = 0;

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
	delete measurer;
	delete locusSelecter;
}

void Player::Initialize()
{
	name = typeid(*this).name();
	onGround = true;
	scale = { 0.9f };
	position = StartPos;	
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };
	cameraRotCount = 0;
	rotCamera = false;
	radY = 0;
	reset = true;
	appear = false;
	resetMove = false;
	appearCounter = 0;
	disappearCounter = 0;
	resetMoveCounter = 0;
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

	nowDrawingLocus = predictStar;
	predictStar->ChangeIsDraw(true);
	isInFever = false;
	measurer->Initialize();
	locusSelecter->Initialize();
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

	//��蒼�����̏���
	if(reset)
	{
		ResetPerform();
		camera->SetTarget(position + Vector3{ 0, 1, 0 });
		if (!appear)
			myModel->PlayAnimation("stand", true, 1, false);
		Object::Update();
		return;//��蒼�����͂����܂�
	}
	
	locusSelecter->Update();

	SelectLocus();
	CheckIsInFever();

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		isDrawing = true;
		//���̐���
		CreateLine();
	}

	//�ړ�����
	Move();
	
	//�h���[�C���O�̏���
	DrawingLine();
	

	//�J�����̃��Z�b�g����
	MoveCamera();

	if (!isDrawing)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::TriggerPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			Attack();
			DeleteLocuss();
		}
	}

	measurer->Update();
	//
	HitCheckLoci();

	////�ǃW�����v����
	//WallJump();
	////�G�A�X���C�h����
	//AirSlide();
	////��i�W�����v����
	//if (!onGround && !secondJump && !wallJump &&(Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(SettingParam::GetJumpButton())))
	//{
	//	secondJump = true;
	//	fallV = { 0,secondJumpVYFist,0,0 };
	//	ParticleEmitter::CreateShock(position);
	//}
	//��������
	if (!onGround)
	{
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];		
		
	}
	////�W�����v����
	//else if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(SettingParam::GetJumpButton())) &&!jump)
	//{
	//	//jump = true;
	//	//onGround = false;
	//	//fallV = { 0,jumpVYFist,0,0 };
	//	ParticleEmitter::CreateShock(position);
	//}
	//���̃I�u�W�F�N�g�Ƃ̃q�b�g�`�F�b�N
	CheckHit();

	if (prePos != position)
	{
		//camera->SetTarget(position + Vector3{0, 1, 0});
	}

	if (!isDrawing)
	{
		//nowDrawingLocus->Move(position, LocusUtility::Vector2ToAngle(direction));
		predictStar->Move(position, LocusUtility::Vector2ToAngle(direction));
		predictRibbon->Move(position, LocusUtility::Vector2ToAngle(direction));
		predictTriforce->Move(position, LocusUtility::Vector2ToAngle(direction));
	}	
}

void Player::Draw()
{
	
	if(resetMoveCounter != 0)
		return;
	if (!Object3D::GetDrawShadow())
	{
		HRESULT result;
		if (reset)
		{
			ConstBuffData* constMap = nullptr;
			result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
			assert(SUCCEEDED(result));
			memcpy(constMap, &sendData, sizeof(ConstBuffData));
			constBuff[bbIndex]->Unmap(0, nullptr);
		}

		//�萔�o�b�t�@���Z�b�g
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constCameraBuff->GetGPUVirtualAddress());
		if (reset)
		{
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, constBuff[bbIndex]->GetGPUVirtualAddress());
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(6, Texture::GetGpuDescHandleSRV("shadowMap" +std::to_string(bbIndex)));  //�q�[�v�̐擪���萔�o�b�t�@
		}
		else
		{
			DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(5, Texture::GetGpuDescHandleSRV("shadowMap" + std::to_string(bbIndex)));  //�q�[�v�̐擪���萔�o�b�t�@
		}
	}
	CustomDraw(true, true);
	if (!Object3D::GetDrawShadow())
	{
		measurer->Draw();
		locusSelecter->Draw();
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

		ImGui::SliderFloat("destruction", &sendData._Destruction, 0, 1.0f);
		ImGui::SliderFloat("scaleFactor", &sendData._ScaleFactor, 0, 1.0f);
		ImGui::SliderFloat("positionFactor", &sendData._PositionFactor, 0, 2.0f);
		ImGui::SliderFloat("rotationFactor", &sendData._RotationFactor, 0, 30.0f);
		ImGui::SliderInt("tessellation", &sendData._Tessellation, 1, 32);
		ImGui::SliderInt("onEasing", &sendData._OnEasing, 0, 1);
		ImGui::Checkbox("reset", &reset);

		ImGui::End();
	}
#endif

	if (Object3D::GetDrawShadow())
		pipelineName = "FBXShadowMap";
	else
	{
		if (reset)
			pipelineName = "FBXPolygonBreak";
		else
			pipelineName = "FBX";

	}
}

void Player::Reset()
{
	//�o���������Ă����ԂȂ��蒼���t���O�𗧂Ă�
	if (appear)
		reset = true;
}

void Player::Move()
{
	prePos = position;
	//�J�����̃r���[�s��̋t�s����v�Z
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//����ƕ����̐؂�ւ�����
	
	if (isDrawing)
	{
		speed = drawingSpeed;
	}
	else
	{
		
		speed = walkSpeed;
	}

	//�ړ�����
	if (Input::DownKey(DIK_A) || Input::DownKey(DIK_D) || Input::DownKey(DIK_S) || Input::DownKey(DIK_W)||
		Input::CheckPadLStickDown()|| Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft() ||
		isInFever)
	{
		if (onGround)
		{
			walkDustCounter++;
			if (walkDustCounter > 25)
			{
				if (!run)
				{
					ParticleEmitter::CreateWalkDust(position, direction);
				}
				else
				{
					ParticleEmitter::CreateRunDust(position, direction);
				}
				walkDustCounter = 0;
			}
		}
		//�ړ�����
		Vector3 moveDirection = {};
		Vector2 stickDirection = {};
		Vector2 lineDirection = {};
		
		if (!isDrawing)
		{	
			//���Ԃ��
			
			if (Input::DownKey(DIK_A))
				moveDirection += cameraDirectionX * -1;
			if (Input::DownKey(DIK_D))
				moveDirection += cameraDirectionX;
			if (Input::DownKey(DIK_S))
				moveDirection += cameraDirectionZ * -1;
			if (Input::DownKey(DIK_W))
				moveDirection += cameraDirectionZ;
			if (Input::CheckPadLStickDown() || Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft())
			{
				auto vec = Input::GetLStickDirection();

				moveDirection = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
			}
			inputAccuracy = 1;
			moveDirection.Normalize();
		}
		else
		{
			if (Input::CheckPadLStickDown() || Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft())
			{
				moveDirection = nowDrawingLocus->GetLine(currentLineNum)->GetVelocity();

				//�X�e�B�b�N�̌���
				auto vec = Input::GetLStickDirection();
				stickDirection.x = (cameraDirectionX * vec.x).x;
				stickDirection.y = (cameraDirectionZ * vec.y).z;
				stickDirection = Vector2::Normalize(stickDirection);

				//���̌���
				auto lineVec = nowDrawingLocus->GetLine(currentLineNum)->GetVelocity();
				lineDirection.x = lineVec.x;
				lineDirection.y = lineVec.z;
				lineDirection = Vector2::Normalize(lineDirection);

				inputAccuracy = Vector2::Dot(stickDirection, lineDirection);

				if (inputAccuracy <= 0)
				{
					inputAccuracy = 0;
				}

				inputAccuracy = Easing::EaseOutCirc(0, 1, 1, inputAccuracy);

			}
			else
			{
				inputAccuracy = 0; //�X�e�B�b�N���͂��Ȃ����瓮���Ȃ�
			}

			//�t�B�[�o�[����������
			if (isInFever)
			{
				moveDirection = nowDrawingLocus->GetLine(currentLineNum)->GetVelocity();
				inputAccuracy = 30.0f;
			}

			moveDirection.Normalize();
		}
		

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

		float rotSpeed = rotateSpeed;
		if (abs(rotY) < 55)
		{
			position += moveDirection * (speed * inputAccuracy);
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

		if (isDrawing)
			myModel->PlayAnimation("run", true);
		else
			myModel->PlayAnimation("walk", true);
	}
	else
	{
			myModel->PlayAnimation("stand", true);
	}
}

void Player::CheckHit()
{
	//�{�b�N�X�R���C�_�[���擾
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//�R���C�_�[�X�V
	Object::Update();
	Ray overDirectionRay;
	overDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() + Vector3(0, boxCollider->GetScale().y-0.01f, 0).ConvertXMVECTOR();
	overDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit overDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(overDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&overDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > overDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = overDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}
	Ray underDirectionRay;
	underDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() - Vector3(0,boxCollider->GetScale().y-0.01f,0).ConvertXMVECTOR();
	underDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit underDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(underDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&underDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > underDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = underDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}

	//�R���C�_�[�X�V
	Object::Update();
	
	//�{�b�N�X�̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += boxCollider->GetScale().y;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//�ڒn���
	if (onGround)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float absDistance = 0.2f;
		//�ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f + absDistance))
		{
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
			Object::Update();
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	//�������
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//���n
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
			Object::Update();
			if (fallV.m128_f32[1] <= fallVYMin +0.3f)
				ParticleEmitter::CreateJumpDust(position);

		}
	}
	class PlayerQueryCallBack :public QueryCallback
	{
	public:
		PlayerQueryCallBack(Box* box) :box(box) {};

		//�Փˎ��̃R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//�N�G���[�Ɏg�p����{�b�N�X
		Box* box = nullptr;
		//�Փˑ���̃R���C�_�[
		BaseCollider* collider = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	//�N�G���[�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//�n�`�Ƃ̌�����S����
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	if (rejectVec.Length() != 0)
	{
		BoxCollider coll = *(BoxCollider*)collider;
		BoxCollider* preCollider = &coll;
		//preCollider->SetOffset(preCollider->GetOffset() - airSlideVel.ConvertXMVECTOR());
		preCollider->Update();
		if (callback.collider->GetMax().x > preCollider->GetMax().x && callback.collider->GetMax().x < boxCollider->GetMax().x ||
			callback.collider->GetMax().x < preCollider->GetMax().x && callback.collider->GetMax().x > boxCollider->GetMax().x)
		{
			callback.move.m128_f32[0] = prePos.x - position.x;

		}
		else if (callback.collider->GetMin().x > preCollider->GetMin().x && callback.collider->GetMin().x < boxCollider->GetMin().x ||
			callback.collider->GetMin().x < preCollider->GetMin().x && callback.collider->GetMin().x > boxCollider->GetMin().x)
		{
			callback.move.m128_f32[0] = prePos.x - position.x;

		}
		if (callback.collider->GetMax().z > preCollider->GetMax().z && callback.collider->GetMax().z < boxCollider->GetMax().z ||
			callback.collider->GetMax().z < preCollider->GetMax().z && callback.collider->GetMax().z > boxCollider->GetMax().z)
		{
			callback.move.m128_f32[2] = prePos.z - position.z;

		}
		else if (callback.collider->GetMin().z > preCollider->GetMin().z && callback.collider->GetMin().z < boxCollider->GetMin().z ||
			callback.collider->GetMin().z < preCollider->GetMin().z && callback.collider->GetMin().z > boxCollider->GetMin().z)
		{
			callback.move.m128_f32[2] = prePos.z - position.z;
		}

	}
	//�����ɂ��r�˕��𓮂���
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];


	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0)
		fallV.m128_f32[1] = 0;

	Object::Update();

}


void Player::MoveCamera()
{
	/*XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();*/

	//�J�����̃��Z�b�g����
	//if ((Input::TriggerKey(DIK_C) || Input::TriggerPadButton(SettingParam::GetResetButton())) && !rotCamera)
	//{
	//	rotCamera = true;
	//	float cosA = direction.Dot(cameraDirectionZ);
	//	if (cosA > 1.0f)
	//		cosA = 1.0f;
	//	else if (cosA < -1.0f)
	//		cosA = -1.0f;
	//	radY = acos(cosA);
	//	const Vector3 CrossVec = direction.Cross(cameraDirectionZ);
	//	if (CrossVec.y < 0)
	//		radY *= -1;
	//	cameraRotCount = 0;
	//	//camera->AddPhi(radY);
	//}
	
	//�J�����̉�]����
	/*if (rotCamera)
	{
		cameraRotCount++;
		float rad = radY / RotTime;
		camera->AddPhi(rad);
		if (cameraRotCount >= RotTime)
			rotCamera = false;
	}*/
}

void Player::ResetPerform()
{
	//�o������
	if (!appear)
	{
		//0-1�ɑ���
		if (appearCounter < 60)
		{
			appearCounter++;
			sendData._Destruction = Easing::Lerp(1, 0, (float)appearCounter / 60.0f);
		}
		if (appearCounter == 60)
		{
			//�o������
			appear = true;
			//��蒼�������I��
			reset = false;
		}
		return;
	}
	//���ŏ���
	if (!resetMove)
	{
		//1-0�Ɍ���
		if (disappearCounter < 60)
		{
			disappearCounter++;
			sendData._Destruction = Easing::Lerp(0, 1, (float)disappearCounter / 60.0f);
		}
		//���ł���������
		if (disappearCounter == 60)
		{
			//�����l�Ɉړ�������t���O�𗧂Ă�
			resetMove = true;
			//���݂̈ʒu���ړ��O�̈ʒu�ɐݒ�
			resetStartPos =  position;
			//�J�����̃r���[�s��̋t�s����v�Z
			XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
			//----------�J�����̉�]�p�x���Z�o---------------
			const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
			float cosA = Vector3(0,0,1).Dot(cameraDirectionZ);
			if (cosA > 1.0f)
				cosA = 1.0f;
			else if (cosA < -1.0f)
				cosA = -1.0f;
			radY = acos(cosA);
			const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
			if (CrossVec.y < 0)
				radY *= -1;
			//-----------------------------------------------
			//�J�����̌��݂̉�]�x��������
			resetPhi = camera->GetPhi();
		}
		return;
	}
	//�����l�ւ̈ړ�����
	if (resetMoveCounter < 60)
	{
		resetMoveCounter++;
		//�ʒu�̍X�V
		position.x = Easing::EaseInOutQuart(resetStartPos.x, StartPos.x,60,resetMoveCounter);
		position.y = Easing::EaseInOutQuart(resetStartPos.y, StartPos.y, 60, resetMoveCounter);
		position.z = Easing::EaseInOutQuart(resetStartPos.z, StartPos.z, 60, resetMoveCounter);
		//�J�����̉�]
		const float phi = Easing::EaseInOutQuart(resetPhi, resetPhi + radY, 60, resetMoveCounter);
		camera->SetPhi(phi);
	}
	//�ړ����������珉����
	if (resetMoveCounter == 60)
	{
		Initialize();
	}

}

void Player::SelectLocus()
{
	if (!isDrawing)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
		{
			nowDrawingLocus->ChangeIsDraw(false);
			nowDrawingLocus = predictStar;//�����͔ėp������Ƃ����񍡂͎���
			nowDrawingLocus->ChangeIsDraw(true);
		}
		else if (Input::TriggerPadButton(XINPUT_GAMEPAD_X))
		{
			nowDrawingLocus->ChangeIsDraw(false);
			nowDrawingLocus = predictRibbon;//�����͔ėp������Ƃ����񍡂͎���
			nowDrawingLocus->ChangeIsDraw(true);
		}
		else if (Input::TriggerPadButton(XINPUT_GAMEPAD_Y))
		{
			nowDrawingLocus->ChangeIsDraw(false);
			nowDrawingLocus = predictTriforce;//�����͔ėp������Ƃ����񍡂͎���
			nowDrawingLocus->ChangeIsDraw(true);
		}
	}
}

void Player::CreateLine()
{
	if (currentLineNum < nowDrawingLocus->GetMaxNumLine())
	{
		Vector3 nowLineVel = nowDrawingLocus->GetLine(currentLineNum)->GetVelocity(); //kokokokoko
		pNowDrawingLine = new Line(position, LocusUtility::Vector2ToAngle(nowLineVel), 0, { 1,1,1,1 }, Vector3(0.5f, 0.7f, 0.7f));
		ObjectManager::GetInstance()->Add(pNowDrawingLine, false);
		vecDrawingLines.push_back(pNowDrawingLine);
	}
	
}

void Player::DrawingLine()
{
	if (pNowDrawingLine != nullptr)
	{
		//�{�^���������Ă��邩�h���[�C���O���͐���L�΂�
		if (Input::CheckPadButton(XINPUT_GAMEPAD_A) && isDrawing)
		{	
			if (isExtendLine)
			{	
				if (isInFever)
				{
					pNowDrawingLine->AddLength(speed * inputAccuracy);
				}
				else if (Input::DownKey(DIK_A) || Input::DownKey(DIK_D) || Input::DownKey(DIK_S) || Input::DownKey(DIK_W) ||
					Input::CheckPadLStickDown() || Input::CheckPadLStickUp() || Input::CheckPadLStickRight() || Input::CheckPadLStickLeft())
				{
					pNowDrawingLine->AddLength(speed * inputAccuracy);
				}
			}

			Vector3 endPos = nowDrawingLocus->GetLine(currentLineNum)->GetEndPos();
			Vector3 pPos = position;

			float lengthNowLine = pNowDrawingLine->GetLength();
			float lengthLocusLine = nowDrawingLocus->GetLine(currentLineNum)->GetLength();

			if (lengthLocusLine - lengthNowLine <= 0.05f) //�}�W�b�N�T�C�R�[
			{
				pNowDrawingLine->SetLength(lengthLocusLine);
				currentLineNum++;
				if (currentLineNum >= nowDrawingLocus->GetMaxNumLine())
				{
					isDrawing = false;
					currentLineNum = 0;
					//�����Ő}�`�Ƃ��ĕۑ����鏈��
					BaseLocus* copyLocus = nullptr;
					switch (nowDrawingLocus->GetType())
					{
					case LocusType::UNDIFINED:
						break;
					case LocusType::STAR:
						copyLocus = new TestStar(*predictStar);
						break;
					case LocusType::RIBBON:
						copyLocus = new TestRibbon(*predictRibbon);
						break;
					case LocusType::TRIFORCE:
						copyLocus = new TestTriforce(*predictTriforce);
						break;
					default:
						break;
					}
					 
					vecLocuss.push_back(copyLocus);
					
					MoveEndDrawing(copyLocus);
					DeleteDrawingLine();
					return;
				}
				position = nowDrawingLocus->GetLine(currentLineNum)->GetStartPos();
				CreateLine();
			}
		}
		else
		{
			isDrawing = false;
			currentLineNum = 0;
			DeleteDrawingLine();
			pNowDrawingLine = nullptr;
		}
	}

}

void Player::DeleteDrawingLine()
{
	for (int i = 0; i < vecDrawingLines.size(); i++)
	{
		ObjectManager::GetInstance()->Remove(vecDrawingLines[i]);
	}
	vecDrawingLines.clear();
}

void Player::DeleteLocuss()
{
	auto end = vecLocuss.size();
	if (end >= 5)
	{
		InFever();
	}

	for (int i = 0; i < end; i++)
	{
		delete vecLocuss[i];
		vecLocuss[i] = nullptr;
	}
	vecLocuss.clear();
}

void Player::MoveEndDrawing(BaseLocus* arg_locus)
{
	Vector3 vec = LocusUtility::AngleToVector2(arg_locus->GetAngle() + 180);
	position = arg_locus->GetLine(arg_locus->GetMaxNumLine() - 1)->GetEndPos();
	position += vec * 2.0f;
}

void Player::Attack()
{
	float value = 2 * vecLocuss.size();
	ActorManager::GetInstance()->GetBoss()->Damage(value);
}

void Player::CheckIsInFever()
{
	if (!isInFever)
	{
		return;
	}

	inFeverTimer->Update();

	if (inFeverTimer->IsTime())
	{
		if (!isDrawing)
		{
			isInFever = false;
		}
	}
}

void Player::InFever()
{
	isInFever = true;
	inFeverTimer->Reset();
}

void Player::HitCheckLoci()
{
	static const float radius = 1.0f;

	if (position == prePos)
	{
		return;
	}

	for (auto locus : vecLocuss)
	{
		for (int i = 0; i < locus->GetMaxNumLine(); i++)
		{
			Line* line = locus->GetLine(i);
			Vector2 AO = LocusUtility::Dim3ToDim2XZ(position - line->GetStartPos());
			Vector2 BO = LocusUtility::Dim3ToDim2XZ(position - line->GetEndPos());
			Vector2 AB = LocusUtility::Dim3ToDim2XZ(line->GetEndPos() - line->GetStartPos());
			Vector2 normalAB = Vector2::Normalize(AB);

			float cross = Vector2::Cross(AO, normalAB);
			if (fabsf(cross) > radius)
			{
				continue;
			}

			float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
			if (multiDot <= 0.0f)
			{
				HitLoci(line);
			}

			if (Vector2::Length(AO) < radius || Vector2::Length(BO) < radius)
			{
				HitLoci(line);
			}
		}
	}
}

void Player::HitLoci(Line* arg_line)
{
	position = prePos;

	if (isDrawing)
	{
		isDrawing = false;
		currentLineNum = 0;
		DeleteDrawingLine();
	}
}

bool Player::IsInFever()
{
	//return isDrawing && Input::CheckPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) && Input::CheckPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	return isInFever;
}