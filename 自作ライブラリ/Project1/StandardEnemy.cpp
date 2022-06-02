#include "StandardEnemy.h"
#include "FBXModel.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "ActorManager.h"
#include "Player.h"
#include "Input.h"

const float INTERVAL_ACTIONTIMER = 180.0f;
const float WALKING = 90.0f;

StandardEnemy::StandardEnemy(Vector3 arg_position, float arg_hitWeight)
{
	initPos = arg_position;
	initWeight = arg_hitWeight;

	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("enemy");
	//���f���̐���
	Create(myModel);
	//�����蔻��(Box)�̐���
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale({ 0.2f,0.5f,0.2f });
	boxCollider->SetOffset({ 0,0.5f,0 ,0 });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	Initialize();
}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	delete walkingTimer;

}

void StandardEnemy::Initialize()
{
	object->Initialize();
	position = initPos;
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	velocity = { 0,0,0 };
	aceel = { 0,0,0 };
	color = { 1,0,0.1f,1 };

	actionTimer->Initialize();
	walkingTimer->Initialize();

	weight = initWeight;
	state = EnemyState::Wait;

}

void StandardEnemy::Update()
{
	myModel->PlayAnimation("stand", true, 1, false);
	actionTimer->Update();
	walkingTimer->Update();

	// �s���p�^�[���̑I��
	if (HitCheckLoci())
	{
		state = EnemyState::Straddle;
	}
	//if (/*�^�b�N�����I�����ĉ��ɂ�������Ȃ�������*/)
	//{
	//	state = EnemyState::CoolAfterRushAttack;
	//}
	//if (/*�v���C���[���^�b�N���̎˒��ɓ�������*/)
	//{
	//	state = EnemyState::RushAttack;
	//}
	if (actionTimer->IsTime())
	{
		state = EnemyState::RandomMove;
	}
	//if (/*�����Ȃ����Ԍo�߂�*/)
	//{
	//	state = EnemyState::Wait;
	//}


	// ���܂����p�^�[���ɂ���ĕ���
	switch (state)
	{
	case EnemyState::Wait:						// �ҋ@

		break;

	case EnemyState::RandomMove:				// �����_���ړ�
		Move();

		break;

	case EnemyState::RushAttack:				// �^�b�N��
		RushAttack();

		break;

	case EnemyState::CoolAfterRushAttack:		// �㌄
		// �^�b�N����̌㌄

		break;

	case EnemyState::Straddle:					// ���񒣂�
		Straddle();

		break;

	default:
		break;
	}

	object->Update();
}

void StandardEnemy::Draw()
{
	ImGui::Begin("enemy");

	ImGui::End();

	PipelineState::SetPipeline("FBX");

	object->Draw(true);
}

void StandardEnemy::DrawReady()
{
	pipelineName = "FBX";
}

void StandardEnemy::Move()
{
	if (isMoved == true)
	{
		//�����_���Ȍ���������
		Vector2 moveDir = { 0,0 };
		moveDir.x = (float)((rand() % 20 - 10) / 10.0f);
		moveDir.y = (float)((rand() % 20 - 10) / 10.0f);

		velocity = { moveDir.x,0,moveDir.y };
		velocity.Normalize();

		speed = 0.2f;

		walkingTimer->Reset();
		isMoved = false;
	}

	position += velocity * speed;

	// �ړ������邩���̍s���Ɉڂ�����actionTimer�����Z�b�g
	if (walkingTimer->IsTime())
	{
		state = EnemyState::Wait;
		actionTimer->Reset();
		isMoved = true;
	}
}

void StandardEnemy::RushAttack()
{
	// �v���C���[�̈ʒu�����ė\�����쁨�ːi
	// �v���C���[�ɓ��������ꍇ���㌄
	// �v���C���[�ɂ������炸���ɂ������Ȃ������ꍇ���㌄
	// ���ɓ��B�����ꍇ�����񒣂���
}

void StandardEnemy::Straddle()
{
	// �X�}�u���̊R�ۂ̂�낯�̃C���[�W
	// ��莞�ԓ��񒣂����と�����������ȕ����Ɣ��΂Ɉړ����đҋ@�֖߂�
	// ���񒣂��Ă���r���Ƀv���C���[���Ԃ����Ă����ꍇ����΂���ė�����
}

void StandardEnemy::HitCheck()
{

}

bool StandardEnemy::HitCheckLoci()
{
	static const float radius = 1.0f;

	std::vector<BaseLocus*> vecLocuss = ActorManager::GetInstance()->GetPlayer()->GetVecLocuss();

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
				return true;
			}

			if (Vector2::Length(AO) < radius || Vector2::Length(BO) < radius)
			{
				return true;
			}
		}
	}

	return false;
}

bool StandardEnemy::IsOnField()
{
	return false;
}

void StandardEnemy::DebugControl()
{
	if (Input::DownKey(DIK_J) || Input::DownKey(DIK_L) || Input::DownKey(DIK_K) || Input::DownKey(DIK_I))
	{

	}
}
