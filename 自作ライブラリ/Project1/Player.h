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
#include "BaseLocus.h"
#include "Timer.h"
#include "NormalWaveMeasurer.h"
#include "LocusSelecter.h"

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
	void Reset();
	bool GetReset() { return reset; }

	//フィーバーかどうか　仮置き
	bool IsInFever();
	
private:
	struct ConstBuffData
	{
		float _Destruction; //分解度合い
		float _ScaleFactor; //スケールの変化量
		float _PositionFactor; //ポジションの変化量
		float _RotationFactor; //回転の変化量
		int _Tessellation;//ポリゴン分割度
		int _OnEasing;//イージングで分解するか
		XMFLOAT2 pad;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // 定数バッファ
	ConstBuffData sendData;
	ComPtr<ID3D12Resource> constCameraBuff; // 定数バッファ

	const unsigned short WALK = 0b1 << 0;
	const unsigned short RUN = 0b1 << 1;
	const unsigned short JUMP = 0b1 << 2;
	const unsigned short SECONDJUMP = 0b1 << 3;
	const unsigned short AIRSLIDE = 0b1 << 4;

	unsigned short attribute = 0b0;

	const float val = 0.4f;
	
	const float secondJumpVYFist = 0.4f*val;//二段ジャンプ時上向き初速
	const float jumpVYFist = 0.5f * val;//ジャンプ時上向き初速
	const float wallJumpVYFist = 0.43f * val;//壁ジャンプ時上向き初速

	//下向き加速
	const float fallAcc = -0.02f * val;
	const float fallVYMin = -0.5f;

	int walkDustCounter = 0;
	
	//初期位置
	const Vector3 StartPos = { 0,-4,0 };
	//移動処理
	void Move();
	//当たり判定
	void CheckHit();
	//カメラの制御
	void MoveCamera();
	//出現消滅時の演出処理
	void ResetPerform();
	
	//ドローイングする図形を選択
	void SelectLocus();
	//線を生成
	void CreateLine();
	//線を書く
	void DrawingLine();
	//書き終わった線を消す
	void DeleteDrawingLine();
	//書いた図形を一気に消す ブレイクと名付けたい※関数名要相談
	void DeleteLocuss();
	//図形を描いた後に瞬間移動させる
	void MoveEndDrawing(BaseLocus* arg_locus);
	//既存の図形との当たり判定
	void HitCheckLoci();
	void HitLoci(Line* arg_line);
	//攻撃
	void Attack();
	//
	void CheckIsInFever();
	void InFever();
	
	ObjectManager* pObjectManager = nullptr;

	LocusSelecter* locusSelecter = nullptr;

	Line* pNowDrawingLine = nullptr;

	BaseLocus* nowDrawingLocus = nullptr;

	TestStar* predictStar = nullptr;
	TestTriforce* predictTriforce = nullptr;
	TestRibbon* predictRibbon = nullptr;	
	NormalWaveMeasurer* measurer;

	//書くフラグ
	bool isDrawing = false;
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
	//フィーバー中かどうか
	bool isInFever;
	Timer* inFeverTimer;

	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	XMVECTOR fallV;
	//現在向いてる方向
	Vector3 direction;
	//移動速度
	float speed = 0.09f;
	const float walkSpeed = 0.09f;
	const float drawingSpeed = 0.18f;
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

	bool reset = true;
	bool appear;
	bool resetMove;
	int appearCounter;//出現時の演出用カウンター
	int disappearCounter;//消滅時の演出用カウンター
	int resetMoveCounter;
	Vector3 resetStartPos;
	float resetPhi;
	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;
private://静的メンバ変数
	static DebugCamera* camera;

};

