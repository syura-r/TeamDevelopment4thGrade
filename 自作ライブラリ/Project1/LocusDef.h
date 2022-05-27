#pragma once
#include "Vector.h"
#include <cstdlib>

enum class LocusType
{
	UNDIFINED = -1,
	TRIANGLE,    //三角形
	RIBBON,      //リボン
	PENTAGON,    //五角形
	STAR,        //星
	HEXAGRAM,    //簡易六芒星
	TRIFORCE,    //トライフォース
};

//図形定義用　線の構造体
struct LocusPointInfo
{	
	Vector3 startPos = Vector3(0, 0, 0);
	Vector3 endPos = Vector3(1, 0, 0);
	//ワールドX軸正方向が0°　時計回りに増える
	float angle = 0;
	float length = 0;
};

#define PI 3.14159265

class LocusUtility
{
public:	
	static float Vector3XZToAngle(const Vector3& arg_vector)
	{
		float angle;
		angle = acos(arg_vector.x / sqrt(arg_vector.x * arg_vector.x + arg_vector.z * arg_vector.z));
		angle = angle * 180.0 / PI;

		if (arg_vector.z > 0)
		{
			angle = 360.0f - angle;
		}
		if (std::isnan(angle))
		{
			angle = 0;
		}
		return angle;
	}

	static Vector3 AngleToVector2(float arg_angle)
	{
		Vector3 result;
		float radian = arg_angle * (PI / 180);
		result = Vector3(cosf(radian), 0, -sinf(radian));
		result.Normalize();
		return result;
	}

	static Vector2 Dim3ToDim2XZ(const Vector3& arg_vec3)
	{
		return Vector2(arg_vec3.x, arg_vec3.z);
	}

	static Vector3 Dim2XZToDim3(const Vector2& arg_vec2, const float arg_y = 0.0f)
	{
		return Vector3(arg_vec2.x, arg_y, arg_vec2.y);
	}
};
