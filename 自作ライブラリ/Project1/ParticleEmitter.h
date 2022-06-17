#pragma once
#include"ParticleManager.h"
#include"Camera.h"
class ParticleEmitter
{
private:
    static ParticleManager* particleManager;

public:
    static void Initialize();

    static void CreateExplosion(const Vector3& pos);
    static void CreateShock(const Vector3& pos, const Vector3& arg_rotation = { XM_PI * 0.5f,0,0 });
    static void CreateGetEffect(const Vector3& pos);
    static void CreateRunDust(const Vector3& pos, const Vector3& direction);

    //パネルカット時のエフェクト(arg_pos:生成場所, arg_vec:進行方向)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    static float GetRandom(float arg_min, float arg_max);
};