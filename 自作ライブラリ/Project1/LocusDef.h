#pragma once
#include "Vector.h"

enum class LocusType
{
	UNDIFINED = -1,
	STAR,
	RIBBON,
	TRIFORCE,
};

struct LocusPointInfo
{	
	Vector3 startPos = Vector3(0, 0, 0);
	Vector3 endPos = Vector3(1, 0, 0);
	float angle = 0;
	float length = 0;
};

#define PI 3.14159265

class LocusUtility
{
public:	
	static float Vector2ToAngle(DirectX::XMFLOAT3 vector)
	{
		float angle;
		angle = acos(vector.x / sqrt(vector.x * vector.x + vector.z * vector.z));
		angle = angle * 180.0 / PI;

		if (vector.z > 0)
		{
			angle = 360.0f - angle;
		}
		if (std::isnan(angle))
		{
			angle = 0;
		}
		return angle;
	}

	static DirectX::XMVECTOR AngleToVector2(float arg_angle)
	{
		DirectX::XMVECTOR result;
		float radian = arg_angle * (PI / 180);
		result = { cos(radian), 0, -sin(radian) };
		DirectX::XMVector3Normalize(result);
		return result;
	}
};
