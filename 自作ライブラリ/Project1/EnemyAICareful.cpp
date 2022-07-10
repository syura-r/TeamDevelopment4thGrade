#include "EnemyAICareful.h"

EnemyAICareful* EnemyAICareful::GetInstance()
{
	static EnemyAICareful* instance = new EnemyAICareful();
	return instance;
}

Vector3 EnemyAICareful::KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece)
{
	return Vector3();
}

Vector3 EnemyAICareful::KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAICareful::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAICareful::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAICareful::ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAICareful::ApproachActorInFever(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

bool EnemyAICareful::StartCutOnSafeTiming(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAICareful::StartCutEnoughActivePieces(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAICareful::StartCutIncludeBonus(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAICareful::StartCutReachFever(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAICareful::StartCutKillActorInFever(StandardEnemy* arg_enemy)
{
	return false;
}

EnemyAILabel EnemyAICareful::GetLabel() const
{
	return EnemyAILabel();
}
