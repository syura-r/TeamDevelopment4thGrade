#pragma once
#include "Object.h"
#include "Timer.h"

class BossRangeAttack : public Object
{
public:
    BossRangeAttack(const Vector3& arg_position, const Vector3& arg_scale, const float arg_durationTime);
    ~BossRangeAttack();

    void Initialize()override;

    void Update()override;

    void Draw()override;

    bool IsActive();
    bool IsFinish();

private:
    Timer* durationTimer;
};