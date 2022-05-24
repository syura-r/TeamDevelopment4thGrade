#pragma once
#include "Object.h"

class BossMissile : public Object
{
public:
    BossMissile(const Vector3& arg_position, const Vector3& arg_velocity, const float arg_speed = 2.0f);
    ~BossMissile();

    void Initialize()override;

    void Update()override;

    void Draw()override;

    float GetRadius()const;

private:
    float speed;
};