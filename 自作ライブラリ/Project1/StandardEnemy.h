#pragma once
#include "Object.h"

class FBXModel;
class Timer;
class Line;

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

    void IsBlow();

    // getter
    float GetWeight() { return weight; }
    Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }
    int GetBlowTime() { return blowTime; }

    // setter
    void SetWeight(float arg_weight) { weight = arg_weight; }
    void SetBlowTime(int arg_blowTime) { blowTime = arg_blowTime; }




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
    void HitLoci(Line* arg_line);
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

    Vector3 prePos;

    //���ʂ̂܂܂�position
    Vector3 virtualityPlanePosition;
    Vector3 preVirtualityPlanePosition;

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
    // ������ђ�
    bool isBlow = false;
    // ������ю���
    int blowTime = 60;

};