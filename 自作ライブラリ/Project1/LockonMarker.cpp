#include "LockonMarker.h"

LockonMarker::LockonMarker()
{
	sprite = new Sprite3D();
}

LockonMarker::~LockonMarker()
{
	delete sprite;
}

void LockonMarker::Initialize()
{
	position = { 0,0,0 };
	isActive = true;
}

void LockonMarker::Draw()
{
	if (isActive)
		sprite->DrawSprite("GamePlay_UI_Maker", position);
}

void LockonMarker::SetParentPosition(const Vector3& arg_parentPosition)
{
	position = arg_parentPosition;
}

void LockonMarker::SetIsActive(const bool arg_isActive)
{
	isActive = arg_isActive;
}
