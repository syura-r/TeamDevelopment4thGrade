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
	
	bool IsAppeared();

private:

	//平面のままのposition
	Vector3 virtualityPlanePosition;	

	//判定発生までの時間
	Timer* appearTimer;

public:
	static const float RADIUS;
};