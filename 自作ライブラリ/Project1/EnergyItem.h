#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class EnergyItem : public Object
{
public:
	EnergyItem(const Vector3& arg_position);
	~EnergyItem();

	void Initialize()override;
	void Update()override;

	void Appear();

	static float GetRadius();
	bool IsAppeared();

private:
	static const float RADIUS;

	//ïΩñ ÇÃÇ‹Ç‹ÇÃposition
	Vector3 virtualityPlanePosition;	

	//îªíËî≠ê∂Ç‹Ç≈ÇÃéûä‘
	Timer* appearTimer;
};