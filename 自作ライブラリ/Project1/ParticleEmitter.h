#pragma once
#include"ParticleManager.h"
#include"Camera.h"
class ParticleEmitter
{
private:
    static ParticleManager* particleManager;

public:
    static void Initialize();

    //�p�l���J�b�g���̃G�t�F�N�g(arg_pos:�����ꏊ, arg_vec:�i�s����)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    //�Փˎ��̃G�t�F�N�g(arg_pos:�����ꏊ)
    static void ShockEffect(const Vector3& arg_pos, const Vector3& arg_color);

    static float GetRandom(float arg_min, float arg_max);
};