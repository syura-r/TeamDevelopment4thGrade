#pragma once
#include "Object.h"
#include "HitPointGauge.h"
#include "Timer.h"

enum class ActionState
{
    Wait,
    ShotMissile,
    ExpandRangeAttack,
    CoolAfterRangAttack
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

    void Reset();

    void Damage(float arg_value);

    std::vector<BossMissile*>& GetMissiles();
    std::vector<BossRangeAttack*>& GetRangeAttacks();

private:
    void ShotMissile();
    void CheckMissilesDuration();
    void ExpandRangeAttack();
    void CheckRangeAttacksDuration();

    float hitPoint = 0;

    HitPointGauge* hpGauge = nullptr;

    ActionState state;
    unsigned int attackCount;
    Timer* actionTimer;
    std::vector<BossMissile*> missiles;
    std::vector<BossRangeAttack*> rangeAttacks;
};

