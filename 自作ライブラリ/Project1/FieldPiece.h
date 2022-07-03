#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

enum class PieceDirection
{
	Lower,
	Upper,
};

class FieldPiece : public Object
{
public:
	FieldPiece(const Vector3& arg_position, const PieceDirection arg_direction);
	~FieldPiece();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	bool IsRidden(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius);
	//���g���؂蔲���ꂽ�Ƃ���Field����Ăяo�����
	void CutOneself(Object* arg_obj);

	static float GetSize();
	static float GetSidewaysLength();
	static float GetLowerTimeOffset();
	static float GetUpperTimeOffset();
	static float GetFullOffset();
	Vector3 GetVirtualityPlanePosition()const;
	static float GetWeight();
	PieceDirection GetPieceDirection()const;
	Vector2 GetArrayIndex()const;
	void SetArrayIndex(const Vector2& arg_index);
	bool IsActive()const;
	void ChangeIsActive(const bool arg_isActive);
	bool IsBlockade()const;
	void ChangeIsBlockade(const bool arg_isBlockade);
	bool IsBonus()const;
	void ChangeIsBonus(const bool arg_isBonus);
	bool IsCutable()const;
	void ChangeIsCutable(const bool arg_flag);
	std::vector<Vector2>& GetPoints();

private:
	static const float SIZE;
	static const float SIDEWAYS_LENGTH;
	static const float LOWER_TIME_OFFSET;
	static const float UPPER_TIME_OFFSET;
	static const float FULL_OFFSET;
	//���ʂ̂܂܂�position
	Vector3 virtualityPlanePosition;
	static const float WEIGHT;
	PieceDirection dir;
	Vector2 arrayIndex;
	bool isActive;
	bool isBlockade;
	//�؂蔲�����l�̍��W
	Vector3 cutterPos;
	//�{�[�i�X�p�l�����ǂ���
	bool isBonus;
	//����
	Timer* reviveTimer;
	//���؂邱�Ƃ��ł��邩
	bool isCutable;

	std::vector<Vector2> points;
	static std::vector<Vector2> basePoints;

	void SetPoints();
	void ChangeColorForRidden();
	void Revival();
};