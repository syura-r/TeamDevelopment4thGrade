#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "TestStar.h"
#include "TestRibbon.h"
#include "TestTriforce.h"
#include "TestTriangle.h"
#include "TestPentagon.h"
#include "TestHexagram.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NormalWaveMeasurer.h"
#include "LocusSelecter.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelItem.h"


class StandardEnemy;
class EnergyItem;
class PanelCutLocus;

class Player :
	public Object
{
public:
	Player();
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;
	static void SetDebugCamera(DebugCamera* cameraPtr) { camera = cameraPtr; }
	
	//�c�@���c���Ă��邩
	bool IsAlive();

	void EndDrawing();
	
	bool IsFall() { return fallFlag; }

	bool IsGameEnd() { return gameEnd; }

	// �G�Ɛ}�`�̔���̂���
	std::vector<BaseLocus*>& GetVecLocuss() { return vecLocuss; };
	int GetGottenPanel() { return gottenPanel; }
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }

	Vector3 GetDirection()const;
	PanelCutLocus* GetPanelCutLocus();
	
private:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	ComPtr<ID3D12Resource> constCameraBuff; // �萔�o�b�t�@


	int walkDustCounter = 0;
	
	//�����ʒu
	const Vector3 StartPos = { 0,-5,0 };
	//�ړ�����
	void Move();
	//�J�����̐���
	void MoveCamera();
	
	//�X���Ŋ��鏈��
	void SlidingDown();
	//�ړ������̌���
	void DecideDirection(Vector3& arg_direction);

	//�h���[�C���O����}�`��I��
	//void SelectLocus();
	//�}�`���Z�b�g����
	//void SetLocus(LocusType arg_LocusType);

	//���𐶐�
	void CreateLine();
	//��������
	void DrawingLine();
	//�����I�������������
	void DeleteDrawingLine();
	//�h���[�C���O�̒��f
	void SuspendDrawing();
	//�������}�`����C�ɏ��� �u���C�N�Ɩ��t���������֐����v���k
	void DeleteLocuss();
	//�}�`��`������ɏu�Ԉړ�������
	void MoveEndDrawing(BaseLocus* arg_locus);
	//�����̐}�`�Ƃ̓����蔻��
	void HitCheckLoci();
	void HitLoci(Line* arg_line);	
	//�t�B�[���h���痎���Ȃ�����
	void StayInTheField();
	void StayOnRemainPanels();
	//�G�Ƃ̓����蔻��
	void HitCheckEnemy();
	void HitEnemy(StandardEnemy* arg_enemy);
	//�A�C�e���Ƃ̓����蔻��
	void HitCheckItems();
	void HitItem(EnergyItem* arg_item);
	void HitPanelItem(PanelItem* arg_panelItem);
	//���񒣂�ɂȂ�
	void StartStand(bool arg_outField = true, Vector3 arg_velocity = {});
	//���񒣂蒆�̏���
	void WithStand();
	//�^�b�N���̏���
	void Tackle();
	//�^�b�N���̒��f
	void SuspendTackle();
	//
	void DischargeGottenPanel(StandardEnemy* arg_enemy);
	//��O�ɗ���
	void Fall();

	
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
	//�h���[�C���O���I������}�`
	std::vector<BaseLocus*> vecLocuss;
	//�����ꎞ�I�ɕۑ����Ă���vector
	std::vector<Line*> vecDrawingLines;		
	unsigned int feverQuota;
	const unsigned int maxFeverQuota = 6;
	Sprite* attackSprite;	

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
	//�����t���O
	bool fallFlag;
	//�����܂ł̗P�\
	int fallEasingCount;

	Vector3 fallStartPos;
	Vector3 fallEndPos;

	//���͊J�n�܂ł̃C���^�[�o��
	int inputStartCount;
	//��{60
	int nextInputStartCount;
	//�߂�ǂ�
	int count;
	//�Q�[���G���h
	bool gameEnd;


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
	//�J������]��
	bool rotCamera;
	//�J�����̉�]�x����
	float radY;
	int cameraRotCount;
	const int RotTime = 10;

	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;

	//�p�l���������\��
	PanelCountUI* panelCountUI = nullptr;

private://�ÓI�����o�ϐ�
	static DebugCamera* camera;

};

