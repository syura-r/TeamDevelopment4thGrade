#pragma once
#include "Vector.h"
#include "Object.h"

//�R���s���[�g�V�F�[�_���g�p���Ȃ�FBX�Ȃǂ̃��f����p�����p�[�e�B�N���p���N���X
//���p����
class ModelParticle : public Object
{
public:
    ModelParticle();
    ~ModelParticle();
    void Initialize() override;
    void Update() override;
    void Draw() override;

protected:
};