#pragma once
#include "Sprite3D.h"

class LockonMarker
{
public:
	LockonMarker();
	~LockonMarker();

	void Initialize();
	void Draw();

	void SetParentPosition(const Vector3& arg_parentPosition);
	void SetIsActive(const bool arg_isActive);

private:
	Sprite3D* sprite = nullptr;
	Vector3 position = {};
	bool isActive = false;
};
