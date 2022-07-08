#include "EnemyAIPositiv.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"

EnemyAIPositiv* EnemyAIPositiv::GetInstance()
{
	static EnemyAIPositiv* instance = new EnemyAIPositiv();
	return instance;
}

Vector3 EnemyAIPositiv::KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	auto gottenPanels = field->GetGottenPieces();

	// �P���؂蔲����Ă��Ȃ������烊�^�[��
	if (gottenPanels.size() <= 0) return arg_velocity;
	// �p�l���Ƃ̋���
	Vector3 distance = { 0,0,0 };
	// ��ԋ߂��p�l���Ƃ̋���
	Vector3 nearestDistance = gottenPanels[0]->GetPosition() - arg_currentPos;
	// ��ԋ߂��p�l���̃|�C���^�i�Čv�Z�p�j
	FieldPiece* nearestPiece = gottenPanels[0];

	// �؂蔲���ꂽ�p�l���̈ʒu�𑖍�
	for (auto panel : gottenPanels)
	{
		distance = panel->GetPosition() - arg_currentPos;

		// ��莩���ɋ߂���������������
		if (distance.Length() < nearestDistance.Length())
		{
			// �����̍X�V
			nearestDistance = distance;
			nearestPiece = panel;
		}
	}

	// �����̋K��l����Ŕ�����
	float specifiedValueDistance = nearestPiece->GetSidewaysLength() * 8;
	float a = nearestDistance.Length();
	// �ł��߂��p�l�����K��l�������������烊�^�[��
	if (nearestDistance.Length() >= specifiedValueDistance) return arg_velocity;

	// ���K��������ԋ߂����ւ̃x�N�g���ƌ��݂̐i�s�����x�N�g���̓���
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = Vector3::Normalize(nearestDistance);
	float dot = Vector3::Dot(VecA, VecB);

	// ���ς̋K��l
	float specifiedValueDot = 0.5f;
	// ���ς̐�Βl���K��l�ȉ��������烊�^�[��
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// �i�s�����̒����i���݂̐i�s�����Ƃ��̋t�����̊O�ς��璼�p�ȃx�N�g�����o���j
	Vector3 reverseVec = { -arg_velocity.x,-arg_velocity.y,-arg_velocity.z };
	reverseVec = Vector3::Normalize(reverseVec);
	Vector3 fixVel = reverseVec.Cross(Vector3::Normalize(arg_velocity));

	// �Z�o�����C���x�N�g���ň�x�m�F����
	KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_currentPos, nearestPiece);

	// �i�s�����𒲐������x�N�g����Ԃ�
	return fixVel;
}

Vector3 EnemyAIPositiv::KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

bool EnemyAIPositiv::StartCutOnSafeTiming(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutEnoughActivePieces(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutIncludeBonus(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutReachFever(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutKillActorInFever(StandardEnemy* arg_enemy)
{
	return false;
}

EnemyAILabel EnemyAIPositiv::GetLabel() const
{
	return EnemyAILabel();
}
