#pragma once
#include "Vector.h"

struct LocusPointInfo
{	
	Vector3 startPos = Vector3(0, 0, 0);
	Vector3 endPos = Vector3(1, 0, 0);
	float angle = 0;
	float length = 0;
};