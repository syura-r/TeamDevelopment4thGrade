#pragma once
#include <array>

#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "BaseLocus.h"
#include "NumberSprite.h"

class StandardEnemy;
class EnergyItem;
class PanelCutLocus;

class SamplePlayer :
	public Object
{
public:
	SamplePlayer();
	~SamplePlayer();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;

	//残機が残っているか
	bool IsAlive();

	void EndDrawing();

	// 敵と図形の判定のため
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();

private:

	//初期位置
	const Vector3 StartPos = { 0,-5,-5 };
	//移動処理
	void Move();

	//傾きで滑る処理
	void SlidingDown();
	//移動方向の決定
	void DecideDirection(Vector3& arg_direction);

	//ドローイングの中断
	void SuspendDrawing();
	//書いた図形を一気に消す ブレイクと名付けたい※関数名要相談
	void DeleteLocuss();
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

	//
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	ObjectManager* pObjectManager = nullptr;

	Line* pNowDrawingLine = nullptr;

	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;

	//書くフラグ
	bool drawingFlag = false;
	//線を伸ばすフラグ
	bool isExtendLine = true;
	//図形の線の番号
	int currentLineNum = 0;
	//線の向きとスティック入力の正確さ 0～1 speedにかける
	float inputAccuracy = 0;
	//線を一時的に保存しておくvector
	std::vector<Line*> vecDrawingLines;
	unsigned int feverQuota;
	const unsigned int maxFeverQuota = 6;

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

	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	XMVECTOR fallV;
	//現在向いてる方向
	Vector3 direction;
	//移動速度
	float speed = 0.09f;
	const float walkSpeed = 0.18f;
	const float drawingSpeed = 0.36f;
	const float blowSpeed = 0.5f;
	//走りフラグ
	bool run = false;
	//回転速度
	float rotateSpeed = 17.5f;
	Vector3 prePos;

	FBXModel* myModel = nullptr;

};