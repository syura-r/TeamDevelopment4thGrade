#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class FieldPiece;

enum class RankEnergyItem
{
	NORMAL,
	SILVER,
	GOLD,
};

class EnergyItem : public Object
{
public:
	EnergyItem(const Vector3& arg_position, const RankEnergyItem arg_rank);
	~EnergyItem();

	void Initialize()override;
	void Update()override;

	void Appear();
	void SlidingDown();
	void StayInTheField();
	void StayOnRemainPanels();
	
	Vector3 GetVirtualityPlanePosition();
	static float GetRadius();
	bool IsAppeared();
	RankEnergyItem GetRank()const;
	FieldPiece* GetRidingPiece()const;
	void SetRidingPiece(FieldPiece* arg_piece);

private:

	//平面のままのposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	static const float RADIUS;

	//判定発生までの時間
	Timer* appearTimer;

	//まるのこの種類
	RankEnergyItem rank;

	FieldPiece* ridingPiece;
};