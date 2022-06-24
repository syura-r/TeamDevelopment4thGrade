#pragma once
#include"ParticleManager.h"
#include"Camera.h"
#include "ObjectManager.h"

class ParticleEmitter
{
private:
    static ParticleManager* particleManager;
    static ObjectManager* p_objectManager;
public:
    static void Initialize();

    //パネルカット時のエフェクト(arg_pos:生成場所, arg_vec:進行方向)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    //衝突時のエフェクト(arg_pos:生成場所)
    static void ShockEffect(const Vector3& arg_pos, const Vector3& arg_color);

    //パネル回収時エフェクト
    static void PieceGetEffect();

private:
    static float GetRandom(float arg_min, float arg_max);
};