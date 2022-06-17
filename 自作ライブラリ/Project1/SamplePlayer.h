#pragma once
#include <array>

#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "BaseLocus.h"
#include "NumberSprite.h"

class StandardEnemy;
class EnergyItem;
class PanelCutLocus;

class SamplePlayer :
	public Object
{
public:
	SamplePlayer();
	~SamplePlayer();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;

	//�c�@���c���Ă��邩
	bool IsAlive();

	void EndDrawing();

	// �G�Ɛ}�`�̔���̂���
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();

private:

	//�����ʒu
	const Vector3 StartPos = { 0,-5,-5 };
	//�ړ�����
	void Move();

	//�X���Ŋ��鏈��
	void SlidingDown();
	//�ړ������̌���
	void DecideDirection(Vector3& arg_direction);

	//�h���[�C���O�̒��f
	void SuspendDrawing();
	//�������}�`����C�ɏ��� �u���C�N�Ɩ��t���������֐����v���k
	void DeleteLocuss();
	//�t�B�[���h���痎���Ȃ�����
	void StayInTheField();
	void StayOnRemainPanels();
	//�G�Ƃ̓����蔻��
	void HitCheckEnemy();
	void HitEnemy(StandardEnemy* arg_enemy);
	//�A�C�e���Ƃ̓����蔻��
	void HitCheckItems();
	void HitItem(EnergyItem* arg_item);
	//���񒣂�ɂȂ�
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//���񒣂蒆�̏���
	void WithStand();
	//�^�b�N���̏���
	void Tackle();
	//�^�b�N���̒��f
	void SuspendTackle();

	//
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	ObjectManager* pObjectManager = nullptr;

	Line* pNowDrawingLine = nullptr;

	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;

	//�����t���O
	bool drawingFlag = false;
	//����L�΂��t���O
	bool isExtendLine = true;
	//�}�`�̐��̔ԍ�
	int currentLineNum = 0;
	//���̌����ƃX�e�B�b�N���͂̐��m�� 0�`1 speed�ɂ�����
	float inputAccuracy = 0;
	//�����ꎞ�I�ɕۑ����Ă���vector
	std::vector<Line*> vecDrawingLines;
	unsigned int feverQuota;
	const unsigned int maxFeverQuota = 6;

	const float RADIUS = 1.0f;
	//�v���C���[�̏d��
	float weight;
	//������ђ�
	bool blowFlag = false;
	// ������ю���
	int blowTime = 60;
	//���񒣂蒆
	bool standingFlag = false;
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
	//�^�b�N����
	bool tackleFlag = false;
	bool tackleHitFlag = false;
	//�^�b�N���̈ړ��ʒu
	Vector3 tackleStartPos;
	Vector3 tackleEndPos;
	int tackleCount = 0;

	//���ʂ̂܂܂�position
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	//�ڒn�t���O
	bool onGround = true;
	//�����x�N�g��
	XMVECTOR fallV;
	//���݌����Ă����
	Vector3 direction;
	//�ړ����x
	float speed = 0.09f;
	const float walkSpeed = 0.18f;
	const float drawingSpeed = 0.36f;
	const float blowSpeed = 0.5f;
	//����t���O
	bool run = false;
	//��]���x
	float rotateSpeed = 17.5f;
	Vector3 prePos;

	FBXModel* myModel = nullptr;

};