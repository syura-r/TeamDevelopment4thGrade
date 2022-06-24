#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelCountSprite3D.h"
#include "IState.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;

class BaseGameActor : public Object
{
public:
	BaseGameActor(const Vector3& arg_pos);
	~BaseGameActor();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw() override;
	virtual void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }
	static void InitCamera();
	//カメラの制御
	static void MoveCamera();

	//切り抜き終了時CircularSawから呼ばれる
	void EndDrawing();
	//切り抜き中に衝突を受けたとき
	void HitOnDrawing();

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
	void HitCheckActors();
	void HitActor(BaseGameActor* arg_actor);
	//アイテムとの当たり判定
	void HitCheckItems();
	void HitEnergyItem(EnergyItem* arg_item);
	void HitPanelItem(PanelItem* arg_panelItem);
	//降ってくる端ブロック
	void HitCheckUnableThroughEdge();
	void HitUnableThroughEdge();
	//降ってくるパネル
	void HitCheckUnableThroughBlock();
	void HitUnableThroughBlock();
	//踏ん張りになる
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//踏ん張り中の処理
	void WithStand();
	//タックルの処理
	void Tackle();
	//タックルの中断
	void SuspendTackle();
	//保持パネルばらまき
	void DischargeGottenPanel(BaseGameActor* arg_actor);
	//場外に落下
	void Fall();
	//イージングでの移動
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

protected:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	static ComPtr<ID3D12Resource> constCameraBuff; // 定数バッファ
	static DebugCamera* camera;
	//カメラ回転中
	static bool rotCamera;
	//カメラの回転度合い
	static float radY;
	static int cameraRotCount;
	static const int RotTime = 10;

	ObjectManager* pObjectManager;
	FBXModel* myModel;
	//初期位置
	const Vector3 StartPos;
	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;
	//書くフラグ
	bool drawingFlag = false;
	const float RADIUS;
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
	//落下フラグ
	bool fallFlag;
	//落下までの猶予
	int fallEasingCount;
	//落下の開始・終了位置
	Vector3 fallStartPos;
	Vector3 fallEndPos;
	//上から降ってくるブロックなどに押しつぶされたかどうか
	bool pressFlag;
	//入力開始までのインターバル
	int inputStartCount;
	//基本60
	int nextInputStartCount;
	//めんどい
	int count;
	//ゲームエンド
	bool gameEnd;

	//前回フレームでの座標
	Vector3 prePos;
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

	//パネル所持数表示
	PanelCountUI* panelCountUI = nullptr;
	PanelCountSprite3D* panelCountSprite3D = nullptr;
	//サウンド用フラグ
	bool fallSoundFlag;
};