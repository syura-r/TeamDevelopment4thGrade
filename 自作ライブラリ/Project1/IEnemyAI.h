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
	virtual void Update(StandardEnemy* arg_enemy, Vector3& arg_velocity, ActionStateLabel& arg_stateLabel) = 0;
	virtual bool IsChangeMoveToCUt(StandardEnemy* arg_enemy) = 0;

	virtual EnemyAILabel GetLabel()const = 0;

protected:
	friend class StandardEnemy;
};