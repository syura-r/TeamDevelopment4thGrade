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

class StandardEnemy;
class EnergyItem;

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
	void Reset();
	bool GetReset() { return reset; }
	
	//�c�@���c���Ă��邩
	bool IsAlive();

	// �G�Ɛ}�`�̔���̂���
	std::vector<BaseLocus*>& GetVecLocuss() { return vecLocuss; };
	float GetWeight() { return weight; }
	Vector3 GetVirtualityPlanePosition()const { return virtualityPlanePosition; }
	
private:
	struct ConstBuffData
	{
		float _Destruction; //����x����
		float _ScaleFactor; //�X�P�[���̕ω���
		float _PositionFactor; //�|�W�V�����̕ω���
		float _RotationFactor; //��]�̕ω���
		int _Tessellation;//�|���S�������x
		int _OnEasing;//�C�[�W���O�ŕ������邩
		XMFLOAT2 pad;
	};
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};

	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // �萔�o�b�t�@
	ConstBuffData sendData;
	ComPtr<ID3D12Resource> constCameraBuff; // �萔�o�b�t�@

	const unsigned short WALK = 0b1 << 0;
	const unsigned short RUN = 0b1 << 1;
	const unsigned short JUMP = 0b1 << 2;
	const unsigned short SECONDJUMP = 0b1 << 3;
	const unsigned short AIRSLIDE = 0b1 << 4;

	unsigned short attribute = 0b0;

	const float val = 0.4f;
	
	const float secondJumpVYFist = 0.4f*val;//��i�W�����v�����������
	const float jumpVYFist = 0.5f * val;//�W�����v�����������
	const float wallJumpVYFist = 0.43f * val;//�ǃW�����v�����������

	//����������
	const float fallAcc = -0.02f * val;
	const float fallVYMin = -0.5f;

	int walkDustCounter = 0;
	
	//�����ʒu
	const Vector3 StartPos = { 0,-5,0 };
	//�ړ�����
	void Move();
	//�����蔻��
	void CheckHit();
	//�J�����̐���
	void MoveCamera();
	//�o�����Ŏ��̉��o����
	void ResetPerform();
	
	//�X���Ŋ��鏈��
	void SlidingDown();
	//�ړ������̌���
	void DecideDirection(Vector3& arg_direction);

	//�h���[�C���O����}�`��I��
	void SelectLocus();
	//�}�`���Z�b�g����
	void SetLocus(LocusType arg_LocusType);

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
	//�G�Ƃ̓����蔻��
	void HitCheckEnemy();
	void HitEnemy(StandardEnemy* arg_enemy);
	//�A�C�e���Ƃ̓����蔻��
	void HitCheckItems();
	void HitItem(EnergyItem* arg_item);
	//���񒣂�ɂȂ�
	void IsStand();
	//���񒣂蒆�̏���
	void WithStand();
	//
	Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);
	//�A�C�e���������u��
	void EmitEnergyItem();

	ObjectManager* pObjectManager = nullptr;
	LocusSelecter* locusSelecter = nullptr;

	Line* pNowDrawingLine = nullptr;
	BaseLocus* nowDrawingLocus = nullptr;

	TestStar* predictStar = nullptr;
	TestTriforce* predictTriforce = nullptr;
	TestRibbon* predictRibbon = nullptr;
	TestTriangle* predictTriangle = nullptr;
	TestPentagon* predictPentagon = nullptr;
	TestHexagram* predictHexagram = nullptr;	

	LocusSelecter::Button pressedButton;

	//�����t���O
	bool isDrawing = false;
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
	bool isBlow = false;
	// ������ю���
	int blowTime = 60;
	//���񒣂蒆
	bool isStanding = false;
	//���񒣂蒆�̗P�\
	int standTime = 120;
	//���񒣂�ɂȂ�O�̃x�N�g��
	Vector3 preStandVec;
	//���񒣂蒆�̐F��ω������邽�߂̒l
	float BGColor = 1;
	//���񒣂蕜�A��
	bool isReturningField = false;
	//���A��̒��n�ʒu
	Vector3 returningStartPos = { 0,0,0 };
	Vector3 returningEndPos = { 0,0,0 };
	//���A�ړ��C�[�W���O�p�̃J�E���g
	int moveEasingCount = 0;

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

	bool reset = true;
	bool appear;
	bool resetMove;
	int appearCounter;//�o�����̉��o�p�J�E���^�[
	int disappearCounter;//���Ŏ��̉��o�p�J�E���^�[
	int resetMoveCounter;
	Vector3 resetStartPos;
	float resetPhi;
	
	//BoxCollider* boxCollider;

	FBXModel* myModel = nullptr;
private://�ÓI�����o�ϐ�
	static DebugCamera* camera;

};

