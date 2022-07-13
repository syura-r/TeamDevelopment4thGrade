#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class FieldPiece;

class FeverInItem : public Object
{
public:
	FeverInItem(const Vector3& arg_position);
	~FeverInItem();

	void Initialize()override;
	void Update()override;

	void Appear();
	void StayInTheField();
	void StayOnRemainPanels();

	Vector3 GetVirtualityPlanePosition();
	static float GetRadius();
	bool IsAppeared();
	FieldPiece* GetRidingPiece()const;
	void SetRidingPiece(FieldPiece* arg_piece);

private:

	//���ʂ̂܂܂�position
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	static const float RADIUS;

	//���蔭���܂ł̎���
	Timer* appearTimer;

	FieldPiece* ridingPiece;
};