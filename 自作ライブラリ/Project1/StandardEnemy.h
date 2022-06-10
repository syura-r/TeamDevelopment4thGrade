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
    // �v���C���[�Ƃ̓����蔻��
    bool RangeCheckPlayer();
    // �t�B�[���h��ɋ��邩
    bool IsOnField();
    void StayInTheField();

    // �f�o�b�O�p����
    void DebugControl();
    // �ړ��ƃ��f���̌��������킹��
    void MatchDir();

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

    const float RADIUS = 1.0f;
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
    // �U������ɓ�������
    bool isAttacked = false;
    // �^�b�N���̍��G���a
    const float AttackRange = 6.0f;
    // �^�b�N���̐���
    const float AttackPower = 5.0f;
};