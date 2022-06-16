#pragma once
#include "Object.h"

class FBXModel;
class Timer;
class Line;
class EnergyItem;
class PanelCutLocus;

enum class EnemyState
{
    Wait,                   // �ҋ@
    RandomMove,             // �����_���ȕ����Ɉړ�
    RushAttack,             // �^�b�N��
    CoolAfterRushAttack,    // �^�b�N���̌㌄
    Straddle,               // ���񒣂�
    Cutting                 // �؂蔲��
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

    // �ړ�
    void Move();


    //���񒣂�ɂȂ�
    void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
    //���񒣂蒆�̏���
    void WithStand();
    // �^�b�N��
    void Tackle();
    //�^�b�N���̒��f
    void SuspendTackle();

    // �e�����蔻��`�F�b�N
    void HitCheck();
    //// �}�`�Ƃ̔���
    //bool HitCheckLoci();
    //void HitLoci(Line* arg_line);
    // �v���C���[�Ƃ̓����蔻��
    bool RangeCheckPlayer();
    // �t�B�[���h��ɋ��邩
    bool IsOnField();
    void StayInTheField();
    void StayOnRemainPanels();

    //---------------------------------------------------------
    // �v���C���[�̈ʒu�����
    void ConfirmPlayerPos();
    // ��ԋ߂��A�C�e���̈ʒu���m�F����
    void ConfirmItemPos();
    // �v���C���[�̈ʒu
    Vector3 playerPos;
    // �A�C�e���̈ʒu
    Vector3 itemPos;

    //�A�C�e���Ƃ̓����蔻��
    void HitCheckItems();
    void HitItem(EnergyItem* arg_item);

    // �ۂ̂�������
    int cutPower;
    // �؂蔲�������s���鐔
    int cuttingSowNum = 1;

    // �����_���ȕ����̌���
    Vector2 RandomDir();
    // �A�C�e���A�v���C���[�̈ʒu�����Ĉړ����������߂�
    Vector3 ThinkDir();
    // �ۂ̂��̐����i�؂蔲���j
    void Cutting();
    // �\����
    PanelCutLocus* panelCutLocus;
    // �؂�t���O
    bool cuttingFlag;

    // �l�������p�l��
    int gottenPanel;

    //�؂蔲���̒��f
    void SuspendCutting();

    //---------------------------------------------------------

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
    //���񒣂蒆�̗P�\
    int standTime = 120;
    //���񒣂�ɂȂ�O�̃x�N�g��
    Vector3 preStandVec;
    //���񒣂蒆�̐F��ω������邽�߂̒l
    float BGColor = 1;
    //���񒣂蕜�A��
    bool returningFieldFlag = false;
    //���A��̒��n�ʒu
    Vector3 returningStartPos = { 0,0,0 };
    Vector3 returningEndPos = { 0,0,0 };
    //���A�ړ��C�[�W���O�p�̃J�E���g
    int moveEasingCount = 0;

    // �؂蔲������
    bool isCutting = false;
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