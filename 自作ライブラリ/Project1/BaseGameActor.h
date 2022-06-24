#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelCountSprite3D.h"
#include "IState.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;

class BaseGameActor : public Object
{
public:
	BaseGameActor(const Vector3& arg_pos);
	~BaseGameActor();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw() override;
	virtual void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }
	static void InitCamera();
	//�J�����̐���
	static void MoveCamera();

	//�؂蔲���I����CircularSaw����Ă΂��
	void EndDrawing();
	//�؂蔲�����ɏՓ˂��󂯂��Ƃ�
	void HitOnDrawing();

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
	void HitCheckActors();
	void HitActor(BaseGameActor* arg_actor);
	//�A�C�e���Ƃ̓����蔻��
	void HitCheckItems();
	void HitEnergyItem(EnergyItem* arg_item);
	void HitPanelItem(PanelItem* arg_panelItem);
	//�~���Ă���[�u���b�N
	void HitCheckUnableThroughEdge();
	void HitUnableThroughEdge();
	//�~���Ă���p�l��
	void HitCheckUnableThroughBlock();
	void HitUnableThroughBlock();
	//���񒣂�ɂȂ�
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//���񒣂蒆�̏���
	void WithStand();
	//�^�b�N���̏���
	void Tackle();
	//�^�b�N���̒��f
	void SuspendTackle();
	//�ێ��p�l���΂�܂�
	void DischargeGottenPanel(BaseGameActor* arg_actor);
	//��O�ɗ���
	void Fall();
	//�C�[�W���O�ł̈ړ�
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

protected:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	static ComPtr<ID3D12Resource> constCameraBuff; // �萔�o�b�t�@
	static DebugCamera* camera;
	//�J������]��
	static bool rotCamera;
	//�J�����̉�]�x����
	static float radY;
	static int cameraRotCount;
	static const int RotTime = 10;

	ObjectManager* pObjectManager;
	FBXModel* myModel;
	//�����ʒu
	const Vector3 StartPos;
	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;
	//�����t���O
	bool drawingFlag = false;
	const float RADIUS;
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
	//�����t���O
	bool fallFlag;
	//�����܂ł̗P�\
	int fallEasingCount;
	//�����̊J�n�E�I���ʒu
	Vector3 fallStartPos;
	Vector3 fallEndPos;
	//�ォ��~���Ă���u���b�N�Ȃǂɉ����Ԃ��ꂽ���ǂ���
	bool pressFlag;
	//���͊J�n�܂ł̃C���^�[�o��
	int inputStartCount;
	//��{60
	int nextInputStartCount;
	//�߂�ǂ�
	int count;
	//�Q�[���G���h
	bool gameEnd;

	//�O��t���[���ł̍��W
	Vector3 prePos;
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

	//�p�l���������\��
	PanelCountUI* panelCountUI = nullptr;
	PanelCountSprite3D* panelCountSprite3D = nullptr;
	//�T�E���h�p�t���O
	bool fallSoundFlag;
};