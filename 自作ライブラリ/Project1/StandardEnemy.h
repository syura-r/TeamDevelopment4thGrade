#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelCountSprite3D.h"
#include "Timer.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;
class Player;

class StandardEnemy
	:public Object
{
public:
	StandardEnemy();
	~StandardEnemy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;	

	void EndDrawing();
	void DischargeGottenPanel(Player* arg_player);

	// �G�Ɛ}�`�̔���̂���
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();

	bool GetStanding() { return standingFlag; }

	bool IsTackle() { return tackleFlag; }

	bool IsDrawing() { return drawingFlag; }

	bool IsFall() { return fallFlag; }

	void StartFall();

	bool GetOutField() { return outFieldFlag; }

	// �������
	void StartBlow();
	void SetBlowTime(int arg_blowTime) { blowTime = arg_blowTime; }

	void HitOnDrawing();

private:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	//�����ʒu
	const Vector3 StartPos = { 0,-5,-15 };
	//�ړ�����
	void Move();

	//�X���Ŋ��鏈��
	void SlidingDown();
	//�ړ������̌���
	void DecideDirection(Vector3& arg_direction);
		
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
	//�p�l���΂�܂�
	void DischargeGottenPanel(StandardEnemy* arg_enemy);
	//��O�ɗ���
	void Fall();

	//
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	ObjectManager* pObjectManager = nullptr;	

	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;

	//�����t���O
	bool drawingFlag = false;

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

	//���݌����Ă����
	Vector3 direction;
	//�ړ����x
	float speed = 0.09f;
	const float walkSpeed = 0.18f;
	const float drawingSpeed = 0.36f;
	const float blowSpeed = 0.5f;
	//��]���x
	float rotateSpeed = 17.5f;
	Vector3 prePos;
	//�J������]��
	bool rotCamera;
	//�J�����̉�]�x����
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	FBXModel* myModel = nullptr;

	//�p�l���������\��
	PanelCountUI* panelCountUI = nullptr;
	PanelCountSprite3D* panelCountSprite3D = nullptr;

	//�����t���O
	bool fallFlag;
	//�����܂ł̗P�\
	int fallEasingCount;

	Vector3 fallStartPos;
	Vector3 fallEndPos;

	bool outFieldFlag;

	//--------------------------------------
	// ����
	Timer* actionTimer = nullptr;
	Timer* walkingTimer = nullptr;

	// �����_���ȕ����̌���
	Vector2 RandomDir();
	// �߂��ɂ��镨�ւ̕���
	Vector2 NearObjDir();

	// �؂蔲�������s���鐔
	int cutPowerLimit = 3;
	// �v���C���[�̈ʒu
	Vector3 playerPos;
	// ��ԋ߂��A�C�e���̈ʒu
	Vector3 itemPos;

	// �v���C���[�̈ʒu�����
	void ConfirmPlayerPos();
	// ��ԋ߂��A�C�e���̈ʒu���m�F����
	void ConfirmItemPos();

	// �^�b�N���̍��G���a
	const float AttackRange = 10.0f;
	// �^�b�N���̐���
	const float AttackPower = 5.0f;

	// �v���C���[���^�b�N���̎˒��ɓ�������
	bool RangeCheckPlayer();
	// �ړ�����
	Vector2 moveDir;

	//--------------------------------------
};