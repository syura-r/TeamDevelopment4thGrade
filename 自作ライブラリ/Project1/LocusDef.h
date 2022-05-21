#pragma once
#include "Vector.h"
#include <cstdlib>

 enum class LocusType
{
	UNDIFINED = -1,
	TRIANGLE,
	RIBBON,
	PENTAGON,
	STAR,
	HEXAGRAM,
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

	static Vector2 Dim3ToDim2XZ(const Vector3& arg_vec3)
	{
		return Vector2(arg_vec3.x, arg_vec3.z);
	}

	static Vector3 Dim2XZToDim3(const Vector3& arg_vec2, const float arg_y = 0.0f)
	{
		return Vector3(arg_vec2.x, arg_y, arg_vec2.y);
	}
};
