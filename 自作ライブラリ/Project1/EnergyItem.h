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

	//���ʂ̂܂܂�position
	Vector3 virtualityPlanePosition;	

	//���蔭���܂ł̎���
	Timer* appearTimer;

public:
	static const float RADIUS;
};