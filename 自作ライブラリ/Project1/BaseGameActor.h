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

class IActionState;
enum class ActionStateLabel;
class EnergyItem;
class PanelItem;
class FieldPiece;
class PanelCutLocus;
class UnableThroughEdge;
class UnableThroughBlock;

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
	static void InitCamera(const Vector3& arg_pos);
	//カメラの制御
	static void MoveCamera(Vector3 arg_dir);

	//---全体---
	//フィールドから落ちない処理
	virtual void StayInTheField(ActionStateLabel& arg_label);
	virtual void StayOnRemainPieces(ActionStateLabel& arg_label, FieldPiece* arg_piece);
	//保持パネルばらまき
	virtual void DischargeGottenPanel(BaseGameActor* arg_actor);
	virtual void UpdatePos();
	//ActionState変更
	void ChangeActionState(IActionState* arg_current, IActionState* arg_next);

	//---Move---
	//移動処理
	virtual void StartMove();
	virtual void OnMove(ActionStateLabel& arg_label);
	virtual void EndMove();
	virtual bool IsChangeMoveToTackle();
	virtual bool IsChangeMoveToBlown();
	virtual bool IsChangeMoveToWithstand();
	virtual bool IsChangeMoveToCut();
	virtual bool IsChangeMoveToFall();

	//---Tackle---
	//タックルの処理
	virtual void StartTackle();
	virtual void OnTackle(ActionStateLabel& arg_label);
	//タックルの中断
	virtual void EndTackle();
	virtual bool IsChangeTackleToMove();
	virtual bool IsChangeTackleToBlown();
	virtual bool IsChangeTackleToWithstand();
	virtual bool IsChangeTackleToFall();

	//---Blown---
	virtual void StartBlown();
	virtual void OnBlown(ActionStateLabel& arg_label);
	virtual void EndBlown();
	virtual bool IsChangeBlownToMove();
	virtual bool IsChangeBlownToWithstand();
	virtual bool IsChangeBlownToFall();

	//---Withstand---
	virtual void StartWithstand();	
	//踏ん張りになる
	virtual void OnWithstand(ActionStateLabel& arg_label);
	virtual void EndWithstand();
	virtual bool IsChangeWithstandToMove();
	virtual bool IsChangeWithstandToFall();

	//---Cut---
	virtual void StartCut();
	virtual void OnCut(ActionStateLabel& arg_label);
	virtual void EndCut();
	//切り抜き終了時CircularSawから呼ばれる
	virtual void CompleteCut();
	//切り抜き中に衝突を受けたとき
	virtual void SuspendCut();
	//
	void ForcedWeight(const int arg_num);
	virtual bool IsChangeCutToMove();
	virtual bool IsChangeCutToBlown();

	//---Fall---
	//場外に落下
	virtual void StartFall();
	virtual void OnFall(ActionStateLabel& arg_label);
	virtual void EndFall();

	//---衝突判定---
	//敵との当たり判定
	virtual void HitCheckActor(BaseGameActor* arg_actor);
	virtual void HitActor(BaseGameActor* arg_actor);
	//アイテムとの当たり判定
	virtual void HitCheckEnergyItem(EnergyItem* arg_energyItem);
	virtual void HitEnergyItem(EnergyItem* arg_energyItem);
	virtual void HitCheckPanelItem(PanelItem* arg_panelItem);
	virtual void HitPanelItem(PanelItem* arg_panelItem);
	//降ってくる端ブロック
	virtual void HitCheckUnableThroughEdge(UnableThroughEdge* arg_edge);
	virtual void HitUnableThroughEdge(UnableThroughEdge* arg_edge);
	//降ってくるパネル
	virtual void HitCheckUnableThroughBlock(UnableThroughBlock* arg_block);
	virtual void HitUnableThroughBlock(UnableThroughBlock* arg_block);

	//---Getter,Setter---
	inline float GetRadius()const {
		return RADIUS;
	}
	inline float GetWeight()const {
		return weight;
	}
	inline Vector3 GetPrePos()const {
		return prePos;
	}
	inline Vector3 GetVirtualityPlanePosition()const {
		return virtualityPlanePosition;
	}
	inline Vector3 GetPreVirtualityPlanePosition()const {
		return preVirtualityPlanePosition;
	}
	inline Vector3 GetDirection()const {
		return direction;
	}
	inline bool IsCrushed()const {
		return isCrushed;
	}
	inline bool IsEndGame()const {
		return isEndGame;
	}
	inline IActionState* GetActionState() {
		return actionState;
	}	
	inline bool IsHitDuringTackle()const {
		return isHitDuringTackle;
	}		
	inline bool IsReturningField()const {
		return isReturningField;
	}
	inline PanelCutLocus* GetPanelCutLocus() {
		return panelCutLocus;
	}
	inline int GetCutPower()const {
		return cutPower;
	}
	inline int GetGottenPanel()const {
		return gottenPanel;
	}

protected:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};
	friend class ActionStateMove;
	friend class ActionStateTackle;
	friend class ActionStateBlown;
	friend class ActionStateWithstand;
	friend class ActionStateCut;
	friend class ActionStateFall;

	static ComPtr<ID3D12Resource> constCameraBuff; // 定数バッファ
	static DebugCamera* camera;
	//カメラ回転中
	static bool rotCamera;
	//カメラの回転度合い
	static float radY;
	static int cameraRotCount;
	static const int ROT_TIME;

	//---全体---
	ObjectManager* pObjectManager;
	FBXModel* myModel;//-//
	//初期位置
	const Vector3 START_POS;
	const float RADIUS;
	//プレイヤーの重さ
	float weight;
	//前回フレームでの座標
	Vector3 prePos;
	//平面のままのposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;
	//現在向いてる方向
	Vector3 direction;
	//パネル所持数表示	
	PanelCountSprite3D* panelCountSprite3D;//-//
	//上から降ってくるブロックなどに押しつぶされたかどうか
	bool isCrushed;
	//ゲームエンド
	bool isEndGame;
	//行動状態
	IActionState* actionState;
	//傾きで滑る処理
	virtual void SlidingDown();
	//イージングでの移動
	virtual Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	//---Move---
	//移動速度
	float speed;
	const float WALK_SPEED;
	const float DRAWING_SPEED;
	const float BLOWN_SPEED;
	//回転速度
	const float ROTATE_SPEED;
	//移動方向の決定
	virtual void DecideDirection(Vector3& arg_direction);

	//---Tackle---
	//タックル中	
	bool isHitDuringTackle;
	//タックルの移動位置
	Vector3 tackleStartPos;
	Vector3 tackleEndPos;
	int tackleCount;

	//---Blown---	
	// 吹っ飛び時間
	int blownTime;

	//---Withstand---	
	//踏ん張り中の猶予
	int withstandTime;
	//踏ん張りになる前のベクトル
	Vector3 preWithstandVec;
	//踏ん張り中の色を変化させるための値
	float BGColor;
	//踏ん張り復帰中
	bool isReturningField;
	//復帰後の着地位置
	Vector3 returningStartPos;
	Vector3 returningEndPos;
	//復帰移動イージング用のカウント
	int moveEasingCount;
	//入力開始までのインターバル
	int inputStartCount;
	//基本60
	int nextInputStartCount;
	int notWithstandCount;

	//---Cut---	
	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;
	int bonusCount;

	//---Fall---	
	//落下までの猶予
	int fallEasingCount;
	//落下の開始・終了位置
	Vector3 fallStartPos;
	Vector3 fallEndPos;
	//サウンド用フラグ
	bool isPlayedFallSound;
};