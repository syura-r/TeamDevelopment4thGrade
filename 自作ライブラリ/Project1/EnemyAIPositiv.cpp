#include "EnemyAIPositiv.h"

EnemyAIPositiv* EnemyAIPositiv::GetInstance()
{
	static EnemyAIPositiv* instance = new EnemyAIPositiv();
	return instance;
}

Vector3 EnemyAIPositiv::KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece)
{
	return Vector3();
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
