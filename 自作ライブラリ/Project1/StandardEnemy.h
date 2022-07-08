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
#include "PanelCountSprite3D.h"
#include "Timer.h"
#include "BaseGameActor.h"

class EnergyItem;
class PanelItem;
class PanelCutLocus;
class Player;
class IEnemyAI;
enum class EnemyAILabel;

class StandardEnemy : public BaseGameActor
{
public:
	StandardEnemy(const Vector3& arg_pos, const EnemyAILabel& arg_AILabel);
	~StandardEnemy();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;	

	//---Move---
	//�ړ�����
	virtual void StartMove()override;
	virtual void OnMove(ActionStateLabel& arg_label)override;
	virtual bool IsChangeMoveToTackle()override;
	virtual bool IsChangeMoveToCut()override;

	//---Tackle---
	virtual void StartTackle()override;

	//---Withstand---
	virtual void StartWithstand()override;
	virtual void OnWithstand(ActionStateLabel& arg_label)override;

	//---Cut---
	//�؂蔲���I����CircularSaw����Ă΂��
	virtual void CompleteCut()override;

protected:
	//---Move---
	//�ړ������̌���
	virtual void DecideDirection(Vector3& arg_direction)override;

	//---Cut---
	virtual void SetTargetActor()override;

	//--------------------------------------
	// ����
	Timer* actionTimer = nullptr;

	// �����_���ȕ����̌���
	Vector2 RandomDir();
	// �߂��ɂ��镨�ւ̕���
	Vector2 NearObjDir();

	// �؂蔲�������s���鐔�̏���A����
	int cutPowerUpperLimit = 3;
	int cutPowerLowerLimit = 1;
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

	//---AI---
	EnemyAILabel enemyAILabel;
	IEnemyAI* enemyAI = nullptr;

	// �ŏ��̈��ړ�����łǂ��ɂ����܂�by�C�C�W�}
	bool firstMoved;
};