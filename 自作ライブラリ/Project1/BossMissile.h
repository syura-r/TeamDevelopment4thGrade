#pragma once
#include "Object.h"

class BossMissile : public Object
{
public:
    BossMissile(const Vector3& arg_position, const Vector3& arg_velocity);
    ~BossMissile();

    void Initialize()override;

    void Update()override;

    void Draw()override;

private:
    float speed;
};