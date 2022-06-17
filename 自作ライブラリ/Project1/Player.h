#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "TestStar.h"
#include "TestRibbon.h"
#include "TestTriforce.h"
#include "TestTriangle.h"
#include "TestPentagon.h"
#include "TestHexagram.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NormalWaveMeasurer.h"
#include "LocusSelecter.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelItem.h"


class StandardEnemy;
class EnergyItem;
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
	
	//残機が残っているか
	bool IsAlive();

	void EndDrawing();
	
	bool IsFall() { return fallFlag; }

	bool IsGameEnd() { return gameEnd; }

	// 敵と図形の判定のため
	std::vector<BaseLocus*>& GetVecLocuss() { return vecLocuss; };
	int GetGottenPanel() { return gottenPanel; }
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();
	
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

	//ドローイングする図形を選択
	//void SelectLocus();
	//図形をセットする
	//void SetLocus(LocusType arg_LocusType);

	//線を生成
	void CreateLine();
	//線を書く
	void DrawingLine();
	//書き終わった線を消す
	void DeleteDrawingLine();
	//ドローイングの中断
	void SuspendDrawing();
	//書いた図形を一気に消す ブレイクと名付けたい※関数名要相談
	void DeleteLocuss();
	//図形を描いた後に瞬間移動させる
	void MoveEndDrawing(BaseLocus* arg_locus);
	//既存の図形との当たり判定
	void HitCheckLoci();
	void HitLoci(Line* arg_line);	
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
	//踏ん張りになる
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//踏ん張り中の処理
	void WithStand();
	//タックルの処理
	void Tackle();
	//タックルの中断
	void SuspendTackle();
	//
	void DischargeGottenPanel(StandardEnemy* arg_enemy);
	//場外に落下
	void Fall();

	
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
	//ドローイングし終わった図形
	std::vector<BaseLocus*> vecLocuss;
	//線を一時的に保存しておくvector
	std::vector<Line*> vecDrawingLines;		
	unsigned int feverQuota;
	const unsigned int maxFeverQuota = 6;
	Sprite* attackSprite;	

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
	//カメラ回転中
	bool rotCamera;
	//カメラの回転度合い
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;

	//パネル所持数表示
	PanelCountUI* panelCountUI = nullptr;

private://静的メンバ変数
	static DebugCamera* camera;

};

