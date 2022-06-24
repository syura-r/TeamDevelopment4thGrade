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
#include "PanelCountUI.h"
#include "PanelCountSprite3D.h"
#include "Timer.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;
class Player;

class StandardEnemy
	:public Object
{
public:
	StandardEnemy();
	~StandardEnemy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;	

	void EndDrawing();
	void DischargeGottenPanel(Player* arg_player);

	// 敵と図形の判定のため
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();

	bool GetStanding() { return standingFlag; }

	bool IsTackle() { return tackleFlag; }

	bool IsDrawing() { return drawingFlag; }

	bool IsFall() { return fallFlag; }

	void StartFall();

	bool GetOutField() { return outFieldFlag; }

	// 吹っ飛び
	void StartBlow();
	void SetBlowTime(int arg_blowTime) { blowTime = arg_blowTime; }

	void HitOnDrawing();

private:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	//初期位置
	const Vector3 StartPos = { 0,-5,-15 };
	//移動処理
	void Move();

	//傾きで滑る処理
	void SlidingDown();
	//移動方向の決定
	void DecideDirection(Vector3& arg_direction);
		
	//フィールドから落ちない処理
	void StayInTheField();
	void StayOnRemainPanels();
	//敵との当たり判定
	void HitCheckEnemy();
	void HitEnemy(StandardEnemy* arg_enemy);
	//アイテムとの当たり判定
	void HitCheckItems();
	void HitItem(EnergyItem* arg_item);
	//踏ん張りになる
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//踏ん張り中の処理
	void WithStand();
	//タックルの処理
	void Tackle();
	//タックルの中断
	void SuspendTackle();
	//パネルばらまき
	void DischargeGottenPanel(StandardEnemy* arg_enemy);
	//場外に落下
	void Fall();

	//
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	ObjectManager* pObjectManager = nullptr;	

	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;

	//書くフラグ
	bool drawingFlag = false;

	const float RADIUS = 1.0f;
	//プレイヤーの重さ
	float weight;
	//吹っ飛び中
	bool blowFlag = false;
	// 吹っ飛び時間
	int blowTime = 60;
	//踏ん張り中
	bool standingFlag = false;
	//踏ん張り中の猶予
	int standTime = 120;
	//踏ん張りになる前のベクトル
	Vector3 preStandVec;
	//踏ん張り中の色を変化させるための値
	float BGColor = 1;
	//踏ん張り復帰中
	bool returningFieldFlag = false;
	//復帰後の着地位置
	Vector3 returningStartPos = { 0,0,0 };
	Vector3 returningEndPos = { 0,0,0 };
	//復帰移動イージング用のカウント
	int moveEasingCount = 0;
	//タックル中
	bool tackleFlag = false;
	bool tackleHitFlag = false;
	//タックルの移動位置
	Vector3 tackleStartPos;
	Vector3 tackleEndPos;
	int tackleCount = 0;


	//平面のままのposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	//現在向いてる方向
	Vector3 direction;
	//移動速度
	float speed = 0.09f;
	const float walkSpeed = 0.18f;
	const float drawingSpeed = 0.36f;
	const float blowSpeed = 0.5f;
	//回転速度
	float rotateSpeed = 17.5f;
	Vector3 prePos;
	//カメラ回転中
	bool rotCamera;
	//カメラの回転度合い
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	FBXModel* myModel = nullptr;

	//パネル所持数表示
	PanelCountUI* panelCountUI = nullptr;
	PanelCountSprite3D* panelCountSprite3D = nullptr;

	//落下フラグ
	bool fallFlag;
	//落下までの猶予
	int fallEasingCount;

	Vector3 fallStartPos;
	Vector3 fallEndPos;

	bool outFieldFlag;

	//--------------------------------------
	// 時間
	Timer* actionTimer = nullptr;
	Timer* walkingTimer = nullptr;

	// ランダムな方向の決定
	Vector2 RandomDir();
	// 近くにある物への方向
	Vector2 NearObjDir();

	// 切り抜きを実行する数
	int cutPowerLimit = 3;
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
};