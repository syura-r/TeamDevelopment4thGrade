#pragma once
#include "IEnemyAI.h"
#include "FieldPiece.h"

class EnemyAIPositiv : public IEnemyAI
{
public:
	static EnemyAIPositiv* GetInstance();

	virtual Vector3 KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece = nullptr) override;
	virtual Vector3 KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity) override;
	virtual Vector3 AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity) override;
	virtual Vector3 ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity) override;
	virtual Vector3 ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity) override;
	virtual Vector3 ApproachActorInFever(StandardEnemy* arg_enemy, const Vector3& arg_velocity) override;

	virtual bool StartCutOnSafeTiming(StandardEnemy* arg_enemy) override;
	virtual bool StartCutEnoughActivePieces(StandardEnemy* arg_enemy) override;
	virtual bool StartCutIncludeBonus(StandardEnemy* arg_enemy) override;
	virtual bool StartCutReachFever(StandardEnemy* arg_enemy) override;
	virtual bool StartCutKillActorInFever(StandardEnemy* arg_enemy) override;

	virtual EnemyAILabel GetLabel()const override;

private:
	EnemyAIPositiv() {}
	~EnemyAIPositiv() {}
	EnemyAIPositiv(const EnemyAIPositiv& another) = delete;
	EnemyAIPositiv& operator=(const EnemyAIPositiv& another) = delete;

private:
	// ‹——£‚Ì‹K’è’l
	const float specifiedValueDistance = FieldPiece::GetSidewaysLength() * 8;
	// “àÏ‚Ì‹K’è’l
	float specifiedValueDot = 0.5f;
	// ŒX‚«‚Ì‹K’è’l
	const float specifiedValueInclination = 1.0f;

	//“_‚Æ•Ó‚Ì‹——£
	float PointToLineDistance(Vector2 arg_point, Vector2 arg_linestart, Vector2 arg_lineend);

	struct ItemRange
	{
		EnergyItem* item;
		float range;

		ItemRange(EnergyItem* arg_item = nullptr, float arg_range = 1.0f)
			:item(arg_item),
			 range(arg_range)
		{
		}
	};

};

