#pragma once
#include "Object.h"
#include "HitPointGauge.h"
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

private:
    float hitPoint = 0;

    HitPointGauge* hpGauge = nullptr;
};

