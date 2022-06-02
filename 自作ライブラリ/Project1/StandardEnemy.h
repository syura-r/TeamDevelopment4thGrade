#pragma once
#include "Object.h"

class FBXModel;
class Timer;

enum class EnemyState
{
    Wait,                   // �ҋ@
    RandomMove,             // �����_���ȕ����Ɉړ�
    RushAttack,             // �^�b�N��
    CoolAfterRushAttack,    // �^�b�N���̌㌄
    Straddle                // ���񒣂�
};

class StandardEnemy :
	public Object
{
public:
    StandardEnemy(Vector3 arg_position, float arg_hitWeight);
    ~StandardEnemy();

    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawReady() override;

    // getter
    float GetWeight() { return weight; }

    // setter
    void SetWeight(float arg_weight) { weight = arg_weight; }

private:
    // �ҋ@

    // �ړ�
    void Move();
    // �^�b�N��
    void RushAttack();
    // ���񒣂�
    void Straddle();

    // �e�����蔻��`�F�b�N
    void HitCheck();
    // �}�`�Ƃ̔���
    bool HitCheckLoci();
    // �t�B�[���h��ɋ��邩
    bool IsOnField();
    void StayInTheField();

    // �f�o�b�O�p����
    void DebugControl();

    // ���f��
    FBXModel* myModel = nullptr;

    // �ǂ�ȍs�������Ă��邩
    EnemyState state;
    // ����
    Timer* actionTimer = nullptr;
    Timer* walkingTimer = nullptr;

    //���ʂ̂܂܂�position
    Vector3 virtualityPlanePosition;

    // �����ʒu
    Vector3 initPos;
    // �����̏d
    float initWeight;
    // �̏d
    float weight;
    // �ړ�������������
    bool isMoved = true;
    // ���x�����p
    float speed = 0.2f;
    // ���񒣂��Ԃ�
    bool isStraddle = false;
    // ���삷�邩
    bool isControl = false;
};