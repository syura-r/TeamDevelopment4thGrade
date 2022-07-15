#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NumberSprite.h"
#include "PanelCountSprite3D.h"
#include "DropPointGetUI.h"
#include "PieChart.h"

class IActionState;
enum class ActionStateLabel;
class EnergyItem;
class PanelItem;
class FieldPiece;
class PanelCutLocus;
class UnableThroughEdge;
class UnableThroughBlock;

class BaseGameActor : public Object
{
public:
	BaseGameActor(const Vector3& arg_pos);
	~BaseGameActor();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw() override;
	virtual void DrawReady() override;
	static void SetCamera(InGameCamera* cameraPtr) { camera = cameraPtr; }
	static void InitCamera(const Vector3& arg_pos);
	//�J�����̐���
	static void MoveCamera(Vector3 arg_dir);

	//---�S��---
	//�t�B�[���h���痎���Ȃ�����
	virtual void StayInTheField(ActionStateLabel& arg_label);
	virtual void StayOnRemainPieces(ActionStateLabel& arg_label, FieldPiece* arg_piece);
	//�ێ��p�l���΂�܂�
	virtual void DischargeGottenPanel(BaseGameActor* arg_actor);
	virtual void UpdatePos();
	//ActionState�ύX
	void ChangeActionState(IActionState* arg_current, IActionState* arg_next);
	//�t�B�[�o�[�ɂ���
	void InFever(const int arg_feverFrame);

	//---Move---
	//�ړ�����
	virtual void StartMove();
	virtual void OnMove(ActionStateLabel& arg_label);
	virtual void EndMove();
	virtual bool IsChangeMoveToTackle();
	virtual bool IsChangeMoveToCut();
	virtual bool IsExistPiecesWithinSawRange();

	//---Tackle---
	//�^�b�N���̏���
	virtual void StartTackle();
	virtual void OnTackle(ActionStateLabel& arg_label);
	//�^�b�N���̒��f
	virtual void EndTackle();

	//---Blown---
	virtual void StartBlown();
	virtual void OnBlown(ActionStateLabel& arg_label);
	virtual void EndBlown();

	//---Withstand---
	virtual void StartWithstand();	
	//���񒣂�ɂȂ�
	virtual void OnWithstand(ActionStateLabel& arg_label);
	virtual void EndWithstand();

	//---Cut---
	virtual void StartCut();
	virtual void OnCut(ActionStateLabel& arg_label);
	virtual void EndCut();
	//�؂蔲���I����CircularSaw����Ă΂��
	virtual void CompleteCut();
	//�؂蔲�����ɏՓ˂��󂯂��Ƃ�
	virtual void SuspendCut();
	//����̏d���𑝂₷
	virtual void ForcedWeight(const int arg_num, BaseGameActor* arg_actor);

	//---Fall---
	//��O�ɗ���
	virtual void StartFall();
	virtual void OnFall(ActionStateLabel& arg_label);
	virtual void EndFall();

	//---Spawn---
	virtual void StartSpawn();
	virtual void OnSpawn(ActionStateLabel& arg_label);
	virtual void EndSpawn();

	//---�Փ˔���---
	//�G�Ƃ̓����蔻��
	virtual void HitCheckActor(BaseGameActor* arg_actor);
	virtual void HitActor(BaseGameActor* arg_actor);
	//�A�C�e���Ƃ̓����蔻��
	virtual void HitCheckEnergyItem(EnergyItem* arg_energyItem);
	virtual void HitEnergyItem(EnergyItem* arg_energyItem);
	virtual void HitCheckPanelItem(PanelItem* arg_panelItem);
	virtual void HitPanelItem(PanelItem* arg_panelItem);
	//�~���Ă���[�u���b�N
	virtual void HitCheckUnableThroughEdge(UnableThroughEdge* arg_edge);
	virtual void HitUnableThroughEdge(UnableThroughEdge* arg_edge);
	//�~���Ă���p�l��
	virtual void HitCheckUnableThroughBlock(UnableThroughBlock* arg_block);
	virtual void HitUnableThroughBlock(UnableThroughBlock* arg_block);

	//---Getter,Setter---
	inline float GetRadius()const {
		return RADIUS;
	}
	inline float GetWeight()const {
		return weight;
	}
	inline Vector3 GetPrePos()const {
		return prePos;
	}
	inline Vector3 GetVirtualityPlanePosition()const {
		return virtualityPlanePosition;
	}
	inline Vector3 GetPreVirtualityPlanePosition()const {
		return preVirtualityPlanePosition;
	}
	inline Vector3 GetDirection()const {
		return direction;
	}
	inline bool IsCrushed()const {
		return isCrushed;
	}
	inline bool IsEndGame()const {
		return isEndGame;
	}
	inline IActionState* GetActionState() {
		return actionState;
	}	
	inline void SetActionState(IActionState* arg_state) {
		this->actionState = arg_state;
	}
	inline int GetKillCount()const {
		return killCount;
	}
	inline void SetKillCount(const int arg_num) {
		killCount = arg_num;
	}
	inline void AddKillCount(const int arg_num) {
		killCount += arg_num;
		dropPointGetUI->PointGet();
	}
	inline void SubtructKillCount(const int arg_num) {
		killCount -= arg_num;
	}
	inline int GetTotalCutCount()const {
		return totalCutCount;
	}
	inline bool IsInFever()const {
		return isInFever;
	}
	inline void SetIsInFever(const bool arg_flag) {
		isInFever = arg_flag;
	}
	inline bool IsHitDuringTackle()const {
		return isHitDuringTackle;
	}		
	inline bool IsReturningField()const {
		return isReturningField;
	}
	inline PanelCutLocus* GetPanelCutLocus() {
		return panelCutLocus;
	}
	inline int GetCutPower()const {
		return cutPower;
	}
	inline int GetGottenPanel()const {
		return gottenPanel;
	}
	inline int GetBonusCount()const {
		return bonusCount;
	}
	inline BaseGameActor* GetTarget()const {
		return targetActor;
	}
	inline DirectX::XMFLOAT4 GetActorColor()const {
		return actorColor;
	}
	inline DirectX::XMFLOAT4 GetEffectColor()const {
		return effectColor;
	}

protected:
	struct ConstLightCameraBuff
	{
		XMMATRIX viewProjection;
		XMFLOAT3 cameraPos;
	};
	friend class ActionStateMove;
	friend class ActionStateTackle;
	friend class ActionStateBlown;
	friend class ActionStateWithstand;
	friend class ActionStateCut;
	friend class ActionStateFall;

	static ComPtr<ID3D12Resource> constCameraBuff; // �萔�o�b�t�@
	//static DebugCamera* camera;
	static InGameCamera* camera;
	//�J������]��
	static bool rotCamera;
	//�J�����̉�]�x����
	static float radY;
	static int cameraRotCount;
	static const int ROT_TIME;

	//---�S��---
	ObjectManager* pObjectManager;
	FBXModel* myModel;//-//
	//�����ʒu
	Vector3 startPos;
	const float RADIUS;
	//�v���C���[�̏d��
	float weight;
	//�O��t���[���ł̍��W
	Vector3 prePos;
	//���ʂ̂܂܂�position
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;
	//���݌����Ă����
	Vector3 direction;
	//�p�l���������\��	
	PanelCountSprite3D* panelCountSprite3D;//-//
	//�ォ��~���Ă���u���b�N�Ȃǂɉ����Ԃ��ꂽ���ǂ���
	bool isCrushed;
	//�Q�[���G���h
	bool isEndGame;
	//�s�����
	IActionState* actionState;
	//���Đ�
	int killCount;
	//���Đ�+1�\��
	DropPointGetUI* dropPointGetUI;
	std::unordered_map<BaseGameActor*, int> weightInfluenceMap;
	PieChart* chart;//-//
	DirectX::XMFLOAT4 actorColor;//-//
	DirectX::XMFLOAT4 effectColor;
	//�Q�[���J�n����؂�����
	int totalCutCount;
	//�t�B�[�o�[���
	bool isInFever;
	Timer* feverTimer;
	//�X���Ŋ��鏈��
	virtual void SlidingDown();
	//�C�[�W���O�ł̈ړ�
	virtual Vector3 EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime);

	//---Move---
	//�ړ����x
	float speed;
	const float WALK_SPEED;
	const float DRAWING_SPEED;
	const float BLOWN_SPEED;
	//��]���x
	const float ROTATE_SPEED;
	//�ړ������̌���
	virtual void DecideDirection(Vector3& arg_direction);

	//---Tackle---
	//�^�b�N����	
	bool isHitDuringTackle;
	//�^�b�N���̈ړ��ʒu
	Vector3 tackleStartPos;
	Vector3 tackleEndPos;
	int tackleCount;

	//---Blown---	
	// ������ю���
	int blownTime;
	//���O�ɓ�����������
	BaseGameActor* pHitActor;
	int hitCount;

	//---Withstand---	
	//���񒣂蒆�̗P�\
	int withstandTime;
	//���񒣂�ɂȂ�O�̃x�N�g��
	Vector3 preWithstandVec;
	//���񒣂蒆�̐F��ω������邽�߂̒l
	float BGColor;
	//���񒣂蕜�A��
	bool isReturningField;
	//���A��̒��n�ʒu
	Vector3 returningStartPos;
	Vector3 returningEndPos;
	//���A�ړ��C�[�W���O�p�̃J�E���g
	int moveEasingCount;
	//���͊J�n�܂ł̃C���^�[�o��
	int inputStartCount;
	//��{60
	int nextInputStartCount;
	int notWithstandCount;

	//---Cut---	
	PanelCutLocus* panelCutLocus;
	int cutPower;
	int gottenPanel;
	int bonusCount;
	BaseGameActor* targetActor;
	int targetIndex;
	virtual void SetTargetActor();

	//---Fall---	
	//�����܂ł̗P�\
	int fallEasingCount;
	//�����̊J�n�E�I���ʒu
	Vector3 fallStartPos;
	Vector3 fallEndPos;
	//�T�E���h�p�t���O
	bool isPlayedFallSound;

	//---Spawn---
	FieldPiece* respawnPiece;
};