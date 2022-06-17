#include "StandardEnemy.h"

#include "Timer.h"
#include "FBXModel.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "ActorManager.h"
#include "Player.h"
#include "Input.h"
#include "Field.h"
#include "FieldPiece.h"
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "PanelCountBoard.h"

const float INTERVAL_ACTIONTIMER = 180.0f;
const float WALKING = 90.0f;

StandardEnemy::StandardEnemy(Vector3 arg_position, float arg_hitWeight)
{
	initPos = arg_position;
	initWeight = arg_hitWeight;

	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("GamePlay_Enemy");
	//���f���̐���
	Create(myModel);

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("StandardEnemy", this);

	panelCountUI = new PanelCountUI();

	ObjectManager::GetInstance()->Add(new PanelCountBoard(this));

	Initialize();
}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	delete walkingTimer;
	delete panelCountUI;
	ActorManager::GetInstance()->DeleteObject(this);
}

void StandardEnemy::Initialize()
{
	object->Initialize();
	position = initPos;
	prePos = position;
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	velocity = { 0,0,0 };
	aceel = { 0,0,0 };
	color = { 1,1,1,1 };

	actionTimer->Initialize();
	walkingTimer->Initialize();

	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	weight = initWeight;
	state = EnemyState::Wait;

	isMoved = true;
	speed = 0.2f;
	isStraddle = false;
	isControl = false;
	isAttacked = false;
	cuttingFlag = false;
	cutPower = 0;
	gottenPanel = 0;
	playerPos = { 0,0,0 };
	itemPos = { 0,0,0 };

	panelCountUI->Initialize();
}

void StandardEnemy::Update()
{
	// �^�C�}�[�X�V
	actionTimer->Update();
	walkingTimer->Update();
	// position�ۑ�
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	// �����ȊO�̈ʒu���X�V
	ConfirmPlayerPos();
	ConfirmItemPos();

	//���藎���鏈��
	float fallSpeed = 0.05f;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	// �t�B�[���h��ɂƂǂ܂鏈��
	StayInTheField();
	StayOnRemainPanels();

	// �����蔻��n
	HitCheckItems();

	// �����ő��삵�������p(imgui�őI��)
	if (isControl)
	{
		DebugControl();
	}

	if (isBlow)
	{
		virtualityPlanePosition += velocity * speed;
		StayInTheField();
		StayOnRemainPanels();

		blowTime--;
		if (blowTime <= 0)
		{
			isBlow = false;
		}
	}
	else
	{
		// �s���p�^�[���̑I��
		
		//if (/*�^�b�N�����I�����ĉ��ɂ�������Ȃ�������*/)
		//{
		//	state = EnemyState::CoolAfterRushAttack;
		//}
		if (RangeCheckPlayer())
		{
			state = EnemyState::RushAttack;
		}
		if (actionTimer->IsTime())
		{
			state = EnemyState::RandomMove;
		}
		//if (/*�����Ȃ����Ԍo�߂�*/)
		//{
		//	state = EnemyState::Wait;
		//}
		// �ۂ̂������������Đ؂蔲��
		if (cutPower >= cuttingSowNum && (!isAttacked && !cuttingFlag))
		{
			state = EnemyState::Cutting;
		}

		// ���܂����p�^�[���ɂ���ĕ���
		switch (state)
		{
		case EnemyState::Wait:						// �ҋ@

			break;

		case EnemyState::RandomMove:				// �����_���ړ�
			Move();

			break;

		case EnemyState::RushAttack:				// �^�b�N��
			Tackle();

			break;

		case EnemyState::CoolAfterRushAttack:		// �㌄
			// �^�b�N����̌㌄

			break;

		case EnemyState::Straddle:					// ���񒣂�
			// ���񒣂�J�n

			break;

		case EnemyState::Cutting:
			Cutting();

			break;
		default:
			break;
		}
	}

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));

	//Vector3 p = field->/*GetPlayerCuttingStartPos()*/;	// ���G�ō�낤

	//if (!cuttingFlag)
	//{
	//	panelCutLocus->SetCutPower(cutPower);
	//	panelCutLocus->Move(p, field->GetPlayerCuttingAngle());
	//}

	object->Update();

	panelCountUI->Update(0);//player��gottenPanel�̖���������ϐ�������
}

void StandardEnemy::Draw()
{
	ImGui::Begin("enemy");
	ImGui::Checkbox("isControl", &isControl);
	ImGui::End();

	PipelineState::SetPipeline("FBX");

	object->Draw(true);

	panelCountUI->Draw(GAMEOBJECT_TYPE::ENEMY);
}

void StandardEnemy::DrawReady()
{
	pipelineName = "FBX";
}

void StandardEnemy::IsBlow()
{
	isBlow = true;
}

void StandardEnemy::Move()
{
	if (isControl)
	{
		return;
	}

	if (isStraddle)
	{
		WithStand();
	}

	if (isMoved == true)
	{
		//�����_���Ȍ���������
		//Vector2 moveDir = RandomDir();
		Vector2 moveDir = { ThinkDir().x,ThinkDir().z };

		velocity = { moveDir.x,0,moveDir.y };

		speed = 0.2f;

		walkingTimer->Reset();
		isMoved = false;
	}

	virtualityPlanePosition += velocity * speed;
	StayInTheField();
	StayOnRemainPanels();

	MatchDir();

	// �ړ������邩���̍s���Ɉڂ�����actionTimer�����Z�b�g
	if (walkingTimer->IsTime())
	{
		state = EnemyState::Wait;
		actionTimer->Reset();
		isMoved = true;
	}
}

void StandardEnemy::StartStand(bool arg_outField, Vector3 arg_velocity)
{
	isStraddle = true;
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

void StandardEnemy::WithStand()
{
	if (isCutting)
	{
		return;
	}

	Vector2 moveDir = RandomDir();

	float accuracy = 0;

	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	accuracy = Vector2::Dot(moveDir, correctVec);

	if (accuracy <= 0)
	{
		accuracy = 0;
	}

	if (accuracy >= 0.55f)
	{
		Vector3 moveDirection = preStandVec;
		Object::SetColor({ 1,1,1,1 });
		returningFieldFlag = true;
		isStraddle = false;
		returningStartPos = virtualityPlanePosition;
		returningEndPos = virtualityPlanePosition + moveDirection * 3;
		return;
	}
}

void StandardEnemy::Tackle()
{
	if (isControl)
	{
		return;
	}

	// �v���C���[�̈ʒu�����ė\�����쁨�ːi
	// �v���C���[�ɓ��������ꍇ���㌄
	// �v���C���[�ɂ������炸���ɂ������Ȃ������ꍇ���㌄
	// ���ɓ��B�����ꍇ�����񒣂���

	if (isAttacked == true)
	{
		state = EnemyState::Wait;
		isAttacked = false;
	}

	if (isAttacked == false)
	{
		// �����̌���
		Vector3 attackDir = ActorManager::GetInstance()->GetPlayer()->GetVirtualityPlanePosition() - virtualityPlanePosition;
		attackDir.Normalize();

		velocity = attackDir * AttackPower;
		isAttacked = true;

		virtualityPlanePosition += velocity * speed;
	}
}

void StandardEnemy::SuspendTackle()
{
}

void StandardEnemy::HitCheck()
{

}

bool StandardEnemy::RangeCheckPlayer()
{
	if (isBlow) { return false; }
	Player* player = ActorManager::GetInstance()->GetPlayer();
	if (!player)
	{
		return false;
	}

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(player->GetVirtualityPlanePosition() - virtualityPlanePosition));
	if (length <= AttackRange)
	{
		return true;
	}

	return false;
}

bool StandardEnemy::IsOnField()
{
	return false;
}

void StandardEnemy::StayInTheField()
{ 
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
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;			
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
			break;
		}
	}
}

void StandardEnemy::StayOnRemainPanels()
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

		if (isAttacked)
		{
			SuspendTackle();
		}
	}
}

void StandardEnemy::ConfirmPlayerPos()
{
	playerPos = ActorManager::GetInstance()->GetPlayer()->GetVirtualityPlanePosition();
}

void StandardEnemy::ConfirmItemPos()
{
	// ActorManager����Item vector�����炤
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();
	// vector���󂾂�����
	if (items.size() <= 0)
	{
		return;
	}

	Vector3 itemDistance;
	Vector3 nearestItemDistance = items[0]->GetPosition() - position;

	// �S�A�C�e���𑖍�
	for (auto item : items)
	{
		itemDistance = item->GetPosition() - position;

		// ��莩���ɋ߂��A�C�e��������������
		if (itemDistance.Length() < nearestItemDistance.Length())
		{
			// �����̍X�V
			nearestItemDistance = itemDistance;
			// ��ԋ߂��A�C�e���̈ʒu���X�V
			itemPos = item->GetPosition();
		}
	}
}

void StandardEnemy::HitCheckItems()
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

void StandardEnemy::HitItem(EnergyItem* arg_item)
{
	arg_item->Dead();
	if (cutPower < 6)
	{
		cutPower++;
	}
}

Vector2 StandardEnemy::RandomDir()
{
	Vector2 dir = { 0,0 };

	dir.x = (float)((rand() % 20 - 10) / 10.0f);
	dir.y = (float)((rand() % 20 - 10) / 10.0f);
	dir = Vector2::Normalize(dir);

	return dir;
}

Vector3 StandardEnemy::ThinkDir()
{
	Vector3 itemRange = itemPos - position;
	Vector3 playerRange = playerPos - position;
	Vector3 dir;
	// �A�C�e�����߂�������A�C�e���̕���
	// �v���C���[���߂�������v���C���[�̕���
	if (itemRange.Length() < playerRange.Length())
	{
		dir = itemPos - position;
		dir.Normalize();
		velocity = dir;
	}
	else
	{
		dir = playerPos - position;
		dir.Normalize();
		velocity = dir;
	}
	
	return velocity;
}

void StandardEnemy::Cutting()
{
	cuttingFlag = true;
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	//�ۂ̂����I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	//Vector3 p = field->GetPlayerCuttingStartPos();	// ���G�ō�낤
	//ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus));

	state = EnemyState::Wait;
}

void StandardEnemy::SuspendCutting()
{
	cuttingFlag = false;
}

void StandardEnemy::DebugControl()
{
	if (isBlow) { return; }
	velocity = { 0,0,0 };

	if (Input::DownKey(DIK_J))
		velocity.x = -1;
	if (Input::DownKey(DIK_L))
		velocity.x = 1;
	if (Input::DownKey(DIK_K))
		velocity.z = -1;
	if (Input::DownKey(DIK_I))
		velocity.z = 1;

	velocity.Normalize();

	virtualityPlanePosition += velocity * speed;
	StayInTheField();
	StayOnRemainPanels();
}

void StandardEnemy::MatchDir()
{
	rotation.y = atan2(velocity.x, velocity.z) * 180 / PI;
}