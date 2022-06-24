#pragma once
#include"ParticleManager.h"
#include"Camera.h"
class ParticleEmitter
{
private:
    static ParticleManager* particleManager;

public:
    static void Initialize();

    //パネルカット時のエフェクト(arg_pos:生成場所, arg_vec:進行方向)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    //衝突時のエフェクト(arg_pos:生成場所)
    static void ShockEffect(const Vector3& arg_pos, const Vector3& arg_color);

    static float GetRandom(float arg_min, float arg_max);
};