#pragma once
#include "StandardEnemy.h"

enum class ActionStateLabel;

enum class EnemyAILabel
{
	POSITIVE,
	CAREFUL,
};

class IEnemyAI
{
public:	
	virtual Vector3 KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;
	virtual Vector3 KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;
	virtual Vector3 AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;
	virtual Vector3 ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;
	virtual Vector3 ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;

	virtual bool StartCutOnSafeTiming(StandardEnemy* arg_enemy) = 0;
	virtual bool StartCutEnoughActivePieces(StandardEnemy* arg_enemy) = 0;
	virtual bool StartCutIncludeBonus(StandardEnemy* arg_enemy) = 0;
	virtual bool StartCutReachFever(StandardEnemy* arg_enemy) = 0;
	virtual bool StartCutKillActorInFever(StandardEnemy* arg_enemy) = 0;

	virtual EnemyAILabel GetLabel()const = 0;

protected:
	friend class StandardEnemy;
};