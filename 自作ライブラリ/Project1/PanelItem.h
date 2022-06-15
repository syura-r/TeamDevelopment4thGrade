#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class PanelItem : public Object
{
public:
	PanelItem(const Vector3& arg_position);
	~PanelItem();

	void Initialize()override;
	void Update()override;

	Vector3 GetVirtualityPlanePosition();
	static float GetRadius();

private:

	//•½–Ê‚Ì‚Ü‚Ü‚Ìposition
	Vector3 virtualityPlanePosition;

	static const float RADIUS;
};