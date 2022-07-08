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
	virtual Vector3 KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece = nullptr) = 0;	// 穴を避けて進行方向を決める
	virtual Vector3 KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;	// 縁を避けて進行方向を決める
	virtual Vector3 AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;		// フィールドの傾きに逆らう
	virtual Vector3 ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;		// 一番近いのこぎりに向かう
	virtual Vector3 ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;	// 切り抜き中のキャラに向かう

	virtual bool StartCutOnSafeTiming(StandardEnemy* arg_enemy) = 0;		// 安全なタイミングで切り抜き
	virtual bool StartCutEnoughActivePieces(StandardEnemy* arg_enemy) = 0;	// ガイド内にパネルがあったら切り抜き
	virtual bool StartCutIncludeBonus(StandardEnemy* arg_enemy) = 0;		// ガイド内のボーナスパネルが規定以上だったら切り抜き
	virtual bool StartCutReachFever(StandardEnemy* arg_enemy) = 0;			// フィーバー間近だったら切り抜き
	virtual bool StartCutKillActorInFever(StandardEnemy* arg_enemy) = 0;	// 自身のフィーバー中は直接狙いに行く

	virtual EnemyAILabel GetLabel()const = 0;

protected:
	friend class StandardEnemy;
};