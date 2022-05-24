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

    //���肪�������Ă��邩
    bool IsActive();
    //�U�����I��������
    bool IsFinish();

private:
    Timer* durationTimer;
};