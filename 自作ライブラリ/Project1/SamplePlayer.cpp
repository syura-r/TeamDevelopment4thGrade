#include "SamplePlayer.h"

#include "Input.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "FieldPiece.h"


SamplePlayer::SamplePlayer()
{
	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("GamePlay_Player");
	//���f���̐���
	Create(myModel);

	pObjectManager = ObjectManager::GetInstance();

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("Player", this);

	Initialize();

}

SamplePlayer::~SamplePlayer()
{
	ActorManager::GetInstance()->DeleteObject(this);
}

void SamplePlayer::Initialize()
{
	onGround = true;
	scale = { 0.9f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };

	drawingFlag = false;
	isExtendLine = false;
	currentLineNum = 0;

	feverQuota = maxFeverQuota;
	virtualityPlanePosition = position;

	preVirtualityPlanePosition = virtualityPlanePosition;
	weight = 10;
	cutPower = 0;
	gottenPanel = 0;
}

void SamplePlayer::Update()
{
	//�ړ�����
	Move();

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
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) && cutPower > 0 && field->GetPlayerRidingPiece())
		{
			if (!tackleFlag && !drawingFlag)
			{
				drawingFlag = true;
				//�ۂ̂����I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
				Vector3 p = field->GetPlayerCuttingStartPos();
				ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus));
			}
		}

		// B�{�^������������
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
		{
			// �^�b�N��
			Tackle();
		}

	}

	if (!drawingFlag)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::TriggerPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			// �}�`�̏����A�t�B�[���h�̃��Z�b�g
			DeleteLocuss();
		}
	}

	//�����蔻��n
	HitCheckEnemy();
	HitCheckItems();

	//���̃I�u�W�F�N�g�Ƃ̃q�b�g�`�F�b�N
	Object::Update();

	Vector3 p = field->GetPlayerCuttingStartPos();

	if (!drawingFlag)
	{
		panelCutLocus->SetCutPower(cutPower);
		panelCutLocus->Move(p, field->GetPlayerCuttingAngle());
	}
}

void SamplePlayer::Draw()
{
	CustomDraw(true, true);
}

void SamplePlayer::DrawReady()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
		ImGui::Begin("PlayerStatus");
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

void SamplePlayer::Move()
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

	// ���񒣂�t���O
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
	//�^�b�N���̈ړ�
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
		StayOnRemainPanels();

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
			StayOnRemainPanels();
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
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void SamplePlayer::SlidingDown()
{
	if (drawingFlag)
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.05f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
	StayOnRemainPanels();
}

void SamplePlayer::DecideDirection(Vector3& arg_direction)
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

	//���蔲�����쒆
	if (drawingFlag)
	{
		arg_direction = { 0,0,0 };
	}
	else
	{
		//if (Input::DownKey(DIK_A))
		//	arg_direction += cameraDirectionX * -1;
		//if (Input::DownKey(DIK_D))
		//	arg_direction += cameraDirectionX;
		//if (Input::DownKey(DIK_S))
		//	arg_direction += cameraDirectionZ * -1;
		//if (Input::DownKey(DIK_W))
		//	arg_direction += cameraDirectionZ;
		//if (Input::CheckPadLStickAnythingDir())
		//{
		//	auto vec = Input::GetLStickDirection();

		//	arg_direction = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
		//}
		inputAccuracy = 1;
	}

	arg_direction.Normalize();
	//�����p�ɑ��
	velocity = arg_direction;
}

void SamplePlayer::SuspendDrawing()
{
	drawingFlag = false;
	currentLineNum = 0;
	pNowDrawingLine = nullptr;
}

void SamplePlayer::DeleteLocuss()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	if (field)
	{
		field->ResetInfluences();
	}
	weight = 5;
	gottenPanel = 0;
}

void SamplePlayer::StayInTheField()
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
			StartStand();
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			StartStand();
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
			StartStand();
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

void SamplePlayer::StayOnRemainPanels()
{

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, RADIUS);

	if (piece)
	{
		virtualityPlanePosition = preVirtualityPlanePosition;

		Vector3 outPieceVec = {};
		outPieceVec = position - piece->GetVirtualityPlanePosition();
		outPieceVec.Normalize();
		StartStand(false, outPieceVec);

		if (tackleFlag)
		{
			SuspendTackle();
		}
	}
}

void SamplePlayer::HitCheckEnemy()
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

void SamplePlayer::HitEnemy(StandardEnemy* arg_enemy)
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
}

void SamplePlayer::HitCheckItems()
{
	if (cutPower >= 6)
	{
		return;
	}

	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();

	for (auto item : items)
	{
		if (!item->IsAppeared())
		{
			continue;
		}

		float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - item->GetVirtualityPlanePosition()));

		if (length <= RADIUS + EnergyItem::GetRadius())
		{
			HitItem(item);
		}
	}
}

void SamplePlayer::HitItem(EnergyItem* arg_item)
{
	arg_item->Dead();
	if (cutPower < 6)
	{
		cutPower++;
	}
}

void SamplePlayer::StartStand(bool arg_outField, Vector3 arg_velocity)
{
	standingFlag = true;
	//�^�b�N���I��
	SuspendTackle();

	standTime = 120;

	//��O���}�`��
	if (arg_outField)
	{
		preStandVec = -position; //�����Ɍ�����
	}
	else
	{
		preStandVec = arg_velocity; //�p�l���̒��S����v���C���[�Ɍ������x�N�g��
	}

	preStandVec.y = 0;
	preStandVec.Normalize();
}

void SamplePlayer::WithStand()
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

	Vector2 stickDirection = {};
	//�X�e�B�b�N�̌���
	auto vec = Input::GetLStickDirection();
	//stickDirection.x = (cameraDirectionX * vec.x).x;
	//stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	float accuracy = 0;

	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	accuracy = Vector2::Dot(stickDirection, correctVec);

	if (accuracy <= 0)
	{
		accuracy = 0;
	}

	if (accuracy >= 0.55f)
	{
		Vector3 moveDirection = preStandVec;
		Object::SetColor({ 1,1,1,1 });
		returningFieldFlag = true;
		standingFlag = false;
		returningStartPos = virtualityPlanePosition;
		returningEndPos = virtualityPlanePosition + moveDirection * 3;
		return;
	}
}

void SamplePlayer::Tackle()
{
	if (tackleFlag || blowFlag || drawingFlag || standingFlag || returningFieldFlag)
	{
		return;
	}

	tackleFlag = true;

	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//�X�e�B�b�N�̌���
	auto vec = Input::GetLStickDirection();
	//stickDirection.x = (cameraDirectionX * vec.x).x;
	//stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	//moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;

}

void SamplePlayer::SuspendTackle()
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
		speed = walkSpeed;
	}

}

Vector3 SamplePlayer::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	Vector3 result = {};
	result.x = Easing::EaseOutCubic(arg_startPos.x, arg_endPos.x, arg_maxTime, arg_nowTime);
	result.y = Easing::EaseOutCubic(arg_startPos.y, arg_endPos.y, arg_maxTime, arg_nowTime);
	result.z = Easing::EaseOutCubic(arg_startPos.z, arg_endPos.z, arg_maxTime, arg_nowTime);
	return result;
}

bool SamplePlayer::IsAlive()
{
	return true;
}

void SamplePlayer::EndDrawing()
{
	drawingFlag = false;
	panelCutLocus->RecordCuttedPanelPos();
	int num = ActorManager::GetInstance()->GetFields()[0]->CutPanel(panelCutLocus->GetCuttedPanelPos());
	weight += num * FieldPiece::GetWeight();
	gottenPanel += num;
	cutPower = 0;
}

Vector3 SamplePlayer::GetDirection() const
{
	return direction;
}

PanelCutLocus* SamplePlayer::GetPanelCutLocus()
{
	return panelCutLocus;
}