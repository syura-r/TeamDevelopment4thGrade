#pragma once
#include "Object.h"

class FBXModel;
class Timer;

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

    // getter
    float GetWeight() { return weight; }

    // setter
    void SetWeight(float arg_weight) { weight = arg_weight; }

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
    // フィールド上に居るか
    bool IsOnField();
    void StayInTheField();

    // デバッグ用操作
    void DebugControl();

    // モデル
    FBXModel* myModel = nullptr;

    // どんな行動をしているか
    EnemyState state;
    // 時間
    Timer* actionTimer = nullptr;
    Timer* walkingTimer = nullptr;

    //平面のままのposition
    Vector3 virtualityPlanePosition;

    // 初期位置
    Vector3 initPos;
    // 初期体重
    float initWeight;
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
};