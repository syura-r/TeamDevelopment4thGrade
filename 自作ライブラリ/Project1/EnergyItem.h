#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

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

private:

	//ïΩñ ÇÃÇ‹Ç‹ÇÃposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	static const float RADIUS;

	//îªíËî≠ê∂Ç‹Ç≈ÇÃéûä‘
	Timer* appearTimer;

	//Ç‹ÇÈÇÃÇ±ÇÃéÌóﬁ
	RankEnergyItem rank;

public:	
};