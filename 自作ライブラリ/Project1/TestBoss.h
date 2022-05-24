#pragma once
#include "Object.h"
#include "HitPointGauge.h"
#include "Timer.h"

enum class ActionState
{
    Wait,                   //�ҋ@
    ShotMissile,            //�~�T�C���𔭎�
    ExpandRangeAttack,      //�͈͍U�����J�n
    CoolAfterRangeAttack    //�͈͍U����̖��s������
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

    //Player����̃_���[�W
    void Damage(float arg_value);
    //�c�@���c���Ă��邩
    bool IsAlive();

    std::vector<BossMissile*>& GetMissiles();
    std::vector<BossRangeAttack*>& GetRangeAttacks();

private:
    //---�~�T�C��---
    void ShotMissile();
    void CheckMissilesDuration();
    //---�͈͍U��---
    void ExpandRangeAttack();
    void CheckRangeAttacksDuration();

    //---HP---
    float hitPoint;
    float maxHitPoint;
    HitPointGauge* hpGauge = nullptr;

    //---�U��---
    //�ǂ�ȍs�������Ă��邩
    ActionState state;
    //����U��������
    unsigned int attackCount;
    Timer* actionTimer;
    std::vector<BossMissile*> missiles;
    std::vector<BossRangeAttack*> rangeAttacks;
};

