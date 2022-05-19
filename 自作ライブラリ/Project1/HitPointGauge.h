#pragma once
#include "Object.h"
class HitPointGauge :
    public Object
{
public:
    HitPointGauge(Vector3 arg_position, float arg_hitPoint, Vector3 arg_scale);
    ~HitPointGauge();

    void Initialize()override;

    void Update()override;

    void Draw()override;

    void SetHitPoint(float arg_hitPoint);

private:

    float hitPoint = 0;
    float maxHitPoint = 0;
};

