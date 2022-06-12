#pragma once
#include "Vector.h"
#include "Quaternion.h"
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
	PANELCUT,
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

//傾きに使う
struct LocusFieldInfluence
{
	Vector3 pos;
	float weight;

	LocusFieldInfluence(const Vector3& arg_pos, const float arg_weight)
		:pos(arg_pos),
		 weight(arg_weight)
	{
	}
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

	static Vector3 ToEuler(const Quaternion& quat)
	{
		static Vector3 result = Vector3();

		result.x = asinf(2 * quat.x * quat.w - 2 * quat.y * quat.z);
		result.y = atan2f(2 * quat.x * quat.z + 2 * quat.y * quat.w, 1.0f - 2 * quat.x * quat.x - 2 * quat.y * quat.y);
		result.z = atan2f(2 * quat.x * quat.y + 2 * quat.z * quat.w, 1.0f - 2 * quat.x * quat.x - 2 * quat.z * quat.z);

		result *= (180.0f / PI);
		return result;
	}

	static Vector3 RotateForFieldTilt(const Vector3& arg_virPos, const Vector3& arg_angleTilt, const Vector3& arg_offset = Vector3())
	{
		Vector3 tmp = arg_virPos - arg_offset;
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(arg_angleTilt.x), XMConvertToRadians(arg_angleTilt.y), XMConvertToRadians(arg_angleTilt.z));
		XMMATRIX posMat = XMMATRIX(tmp.ConvertXMVECTOR(), XMVECTOR(), XMVECTOR(), XMVECTOR());

		posMat *= rotMat;
		tmp = { posMat.r[0].m128_f32[0], posMat.r[0].m128_f32[1] , posMat.r[0].m128_f32[2] };
		tmp += arg_offset;

		return tmp;
	}

	static float Cross3p(const Vector2& p1, const Vector2& p2, const Vector2& p3)
	{
		return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	}
};
