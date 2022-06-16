#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class PanelItem : public Object
{
public:
	PanelItem(const Vector3& arg_position, const Vector3& arg_velocity);
	~PanelItem();

	void Initialize()override;
	void Update()override;

	void Bounce();
	void SlidingDown();
	void StayInTheField();
	void StayOnRemainPanels();

	Vector3 GetVirtualityPlanePosition();
	bool IsEndBounce();
	static float GetRadius();

private:
	//•½–Ê‚Ì‚Ü‚Ü‚Ìposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;

	float bouncePower;

	static const float RADIUS;
	static const float BOUNCE_SPEED;
};