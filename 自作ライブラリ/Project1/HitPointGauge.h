#pragma once
#include "Object.h"
class HitPointGauge :
    public Object
{
public:
    HitPointGauge(Vector3 arg_position, Vector3 arg_scale, float arg_hitPoint);
    ~HitPointGauge();

    void Initialize()override;

    void Update()override;

    void Draw()override;

    void SetHitPoint(float arg_hitPoint);

private:

    float hitPoint;
    float maxHitPoint;
    float maxScale;
};

