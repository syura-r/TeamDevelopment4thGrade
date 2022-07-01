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

    static void SetObjectManager(ObjectManager* arg_objManager);

    //�p�l���J�b�g���̃G�t�F�N�g(arg_pos:�����ꏊ, arg_vec:�i�s����)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    //�Փˎ��̃G�t�F�N�g(arg_pos:�����ꏊ)
    static void ShockEffect(const Vector3& arg_pos, const Vector3& arg_color);

    //�p�l��������G�t�F�N�g
    static void PieceGetEffect(const Vector3& arg_position, const Vector3& arg_scale, const Vector3& arg_rotation, const DirectX::XMFLOAT4& arg_color, Object* arg_getActorObj);

private:
    static float GetRandom(float arg_min, float arg_max);
};