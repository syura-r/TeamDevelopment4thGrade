#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

class Field;

class UnableThroughEdge : public Object
{
public:
	UnableThroughEdge(const Vector3& arg_position, const float arg_emitHeight);
	~UnableThroughEdge();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void FallDown();

	Vector3 GetVirtualityPlanePosition()const;		
	static float GetWeight();
	static float GetHeightMagnification();
	bool IsEndFallDown()const;

private:
	//•½–Ê‚Ì‚Ü‚Ü‚Ìposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;		
	float fallSpeed;
	static const float WEIGHT;
	static const float HEIGHT_MAGNIFICATION;
};