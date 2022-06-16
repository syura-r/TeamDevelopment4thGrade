#pragma once
#include "Object.h"

class FBXModel;
class Timer;
class Line;
class EnergyItem;
class PanelCutLocus;

enum class EnemyState
{
    Wait,                   // 待機
    RandomMove,             // ランダムな方向に移動
    RushAttack,             // タックル
    CoolAfterRushAttack,    // タックルの後隙
    Straddle,               // 踏ん張り
    Cutting                 // 切り抜き
};

class StandardEnemy :
	public Object
{
public:
    StandardEnemy(Vector3 arg_position, float arg_hitWeight);
    ~StandardEnemy();

    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawReady() override;

    void IsBlow();

    // getter
    float GetWeight() { return weight; }
    Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }
    int GetBlowTime() { return blowTime; }

    // setter
    void SetWeight(float arg_weight) { weight = arg_weight; }
    void SetBlowTime(int arg_blowTime) { blowTime = arg_blowTime; }

private:

    // 移動
    void Move();


    //踏ん張りになる
    void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
    //踏ん張り中の処理
    void WithStand();
    // タックル
    void Tackle();
    //タックルの中断
    void SuspendTackle();

    // 各当たり判定チェック
    void HitCheck();
    //// 図形との判定
    //bool HitCheckLoci();
    //void HitLoci(Line* arg_line);
    // プレイヤーとの当たり判定
    bool RangeCheckPlayer();
    // フィールド上に居るか
    bool IsOnField();
    void StayInTheField();
    void StayOnRemainPanels();

    //---------------------------------------------------------
    // プレイヤーの位置を取る
    void ConfirmPlayerPos();
    // 一番近いアイテムの位置を確認する
    void ConfirmItemPos();
    // プレイヤーの位置
    Vector3 playerPos;
    // アイテムの位置
    Vector3 itemPos;

    //アイテムとの当たり判定
    void HitCheckItems();
    void HitItem(EnergyItem* arg_item);

    // 丸のこ所持数
    int cutPower;
    // 切り抜きを実行する数
    int cuttingSowNum = 1;

    // ランダムな方向の決定
    Vector2 RandomDir();
    // アイテム、プレイヤーの位置を見て移動方向を決める
    Vector3 ThinkDir();
    // 丸のこの生成（切り抜き）
    void Cutting();
    // 予測線
    PanelCutLocus* panelCutLocus;
    // 切るフラグ
    bool cuttingFlag;

    // 獲得したパネル
    int gottenPanel;

    //切り抜きの中断
    void SuspendCutting();

    //---------------------------------------------------------

    // デバッグ用操作
    void DebugControl();
    // 移動とモデルの向きを合わせる
    void MatchDir();

    // モデル
    FBXModel* myModel = nullptr;

    // どんな行動をしているか
    EnemyState state;
    // 時間
    Timer* actionTimer = nullptr;
    Timer* walkingTimer = nullptr;

    Vector3 prePos;

    //平面のままのposition
    Vector3 virtualityPlanePosition;
    Vector3 preVirtualityPlanePosition;

    // 初期位置
    Vector3 initPos;
    // 初期体重
    float initWeight;

    const float RADIUS = 1.0f;
    // 体重
    float weight;
    // 移動が完了したか
    bool isMoved = true;
    // 速度調整用
    float speed = 0.2f;

    // 踏ん張り状態か
    bool isStraddle = false;
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

    // 切り抜き中か
    bool isCutting = false;
    // 操作するか
    bool isControl = false;
    // 吹っ飛び中
    bool isBlow = false;
    // 吹っ飛び時間
    int blowTime = 60;
    // 攻撃動作に入ったか
    bool isAttacked = false;
    // タックルの索敵半径
    const float AttackRange = 6.0f;
    // タックルの勢い
    const float AttackPower = 5.0f;
};