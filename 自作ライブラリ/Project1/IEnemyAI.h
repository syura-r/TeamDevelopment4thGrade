#pragma once
#include "StandardEnemy.h"

enum class ActionStateLabel;

enum class EnemyAILabel
{
	POSITIVE,
	CAREFUL,
};

class IEnemyAI
{
public:	
	virtual Vector3 KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece = nullptr) = 0;	// ��������Đi�s���������߂�
	virtual Vector3 KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;	// ��������Đi�s���������߂�
	virtual Vector3 AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;		// �t�B�[���h�̌X���ɋt�炤
	virtual Vector3 ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;		// ��ԋ߂��̂�����Ɍ�����
	virtual Vector3 ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity) = 0;	// �؂蔲�����̃L�����Ɍ�����

	virtual bool StartCutOnSafeTiming(StandardEnemy* arg_enemy) = 0;		// ���S�ȃ^�C�~���O�Ő؂蔲��
	virtual bool StartCutEnoughActivePieces(StandardEnemy* arg_enemy) = 0;	// �K�C�h���Ƀp�l������������؂蔲��
	virtual bool StartCutIncludeBonus(StandardEnemy* arg_enemy) = 0;		// �K�C�h���̃{�[�i�X�p�l�����K��ȏゾ������؂蔲��
	virtual bool StartCutReachFever(StandardEnemy* arg_enemy) = 0;			// �t�B�[�o�[�ԋ߂�������؂蔲��
	virtual bool StartCutKillActorInFever(StandardEnemy* arg_enemy) = 0;	// ���g�̃t�B�[�o�[���͒��ڑ_���ɍs��

	virtual EnemyAILabel GetLabel()const = 0;

protected:
	friend class StandardEnemy;
};