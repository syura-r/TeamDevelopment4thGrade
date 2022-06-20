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


class StandardEnemy;
class EnergyItem;
class PanelItem;
class PanelCutLocus;

class Player :
	public Object
{
public:
	Player();
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }	

	void EndDrawing();
	
	bool IsFall() { return fallFlag; }

	bool IsGameEnd() { return gameEnd; }

	// 敵と図形の判定のため	
	int GetGottenPanel() { return gottenPanel; }
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();

	void HitOnDrawing();
	
private:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	ComPtr<ID3D12Resource> constCameraBuff; // 定数バッファ


	int walkDustCounter = 0;
	
	//初期位置
	const Vector3 StartPos = { 0,-5,0 };
	//移動処理
	void Move();
	//カメラの制御
	void MoveCamera();
	
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
	//落下フラグ
	bool fallFlag;
	//落下までの猶予
	int fallEasingCount;

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

	//サウンド用フラグ
	bool fallSoundFlag;

private://静的メンバ変数
	static DebugCamera* camera;

};

