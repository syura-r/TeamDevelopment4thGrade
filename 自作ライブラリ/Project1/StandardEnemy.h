#pragma once
#include "Object.h"

class FBXModel;
class Timer;
class Line;

enum class EnemyState
{
    Wait,                   // 待機
    RandomMove,             // ランダムな方向に移動
    RushAttack,             // タックル
    CoolAfterRushAttack,    // タックルの後隙
    Straddle                // 踏ん張り
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
    // 待機

    // 移動
    void Move();
    // タックル
    void RushAttack();
    // 踏ん張り
    void Straddle();

    // 各当たり判定チェック
    void HitCheck();
    // 図形との判定
    bool HitCheckLoci();
    void HitLoci(Line* arg_line);
    // プレイヤーとの当たり判定
    bool RangeCheckPlayer();
    // フィールド上に居るか
    bool IsOnField();
    void StayInTheField();

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