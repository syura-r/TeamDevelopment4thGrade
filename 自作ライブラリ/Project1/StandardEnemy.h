#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "NumberSprite.h"
#include "PanelCountSprite3D.h"
#include "Timer.h"
#include "BaseGameActor.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;
class Player;
class IEnemyAI;
enum class EnemyAILabel;

class StandardEnemy : public BaseGameActor
{
public:
	StandardEnemy(const Vector3& arg_pos, const EnemyAILabel& arg_AILabel);
	~StandardEnemy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;	

	//---Move---
	//移動処理
	virtual void StartMove()override;
	virtual void OnMove(ActionStateLabel& arg_label)override;
	virtual bool IsChangeMoveToTackle()override;
	virtual bool IsChangeMoveToCut()override;

	//---Tackle---
	virtual void StartTackle()override;

	//---Withstand---
	virtual void StartWithstand()override;
	virtual void OnWithstand(ActionStateLabel& arg_label)override;

	//---Cut---
	//切り抜き終了時CircularSawから呼ばれる
	virtual void CompleteCut()override;

protected:
	//---Move---
	//移動方向の決定
	virtual void DecideDirection(Vector3& arg_direction)override;

	//---Cut---
	virtual void SetTargetActor()override;

	//--------------------------------------
	// 時間
	Timer* actionTimer = nullptr;

	// ランダムな方向の決定
	Vector2 RandomDir();
	// 近くにある物への方向
	Vector2 NearObjDir();

	// 切り抜きを実行する数の上限、下限
	int cutPowerUpperLimit = 3;
	int cutPowerLowerLimit = 1;
	// プレイヤーの位置
	Vector3 playerPos;
	// 一番近いアイテムの位置
	Vector3 itemPos;

	// プレイヤーの位置を取る
	void ConfirmPlayerPos();
	// 一番近いアイテムの位置を確認する
	void ConfirmItemPos();

	// タックルの索敵半径
	const float AttackRange = 10.0f;
	// タックルの勢い
	const float AttackPower = 5.0f;

	// プレイヤーがタックルの射程に入ったか
	bool RangeCheckPlayer();
	// 移動方向
	Vector2 moveDir;
	//--------------------------------------

	//---AI---
	EnemyAILabel enemyAILabel;
	IEnemyAI* enemyAI = nullptr;

	// 最初の一回移動←後でどうにかしますbyイイジマ
	bool firstMoved;
};