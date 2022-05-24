#pragma once
#include "Object.h"
#include "HitPointGauge.h"
#include "Timer.h"

enum class ActionState
{
    Wait,                   //待機
    ShotMissile,            //ミサイルを発射
    ExpandRangeAttack,      //範囲攻撃を開始
    CoolAfterRangeAttack    //範囲攻撃後の無行動時間
};

class BossMissile;
class BossRangeAttack;

class TestBoss :
    public Object
{
public:
    TestBoss(Vector3 arg_position, float arg_hitPoint);
    ~TestBoss();

    void Initialize()override;
    void Update()override;
    void Draw()override;

    //Playerからのダメージ
    void Damage(float arg_value);
    //残機が残っているか
    bool IsAlive();

    std::vector<BossMissile*>& GetMissiles();
    std::vector<BossRangeAttack*>& GetRangeAttacks();

private:
    //---ミサイル---
    void ShotMissile();
    void CheckMissilesDuration();
    //---範囲攻撃---
    void ExpandRangeAttack();
    void CheckRangeAttacksDuration();

    //---HP---
    float hitPoint;
    float maxHitPoint;
    HitPointGauge* hpGauge = nullptr;

    //---攻撃---
    //どんな行動をしているか
    ActionState state;
    //何回攻撃したか
    unsigned int attackCount;
    Timer* actionTimer;
    std::vector<BossMissile*> missiles;
    std::vector<BossRangeAttack*> rangeAttacks;
};

