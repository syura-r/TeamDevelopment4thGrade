#pragma once
#include "Object.h"
#include "OBJModel.h"

class Line : public Object
{
public:

	Line(Vector3 arg_startPos, float arg_angle, float arg_length, Vector4 arg_color);

	~Line();
	void Initialize()override;

	void Update()override;

	void Reset();

	void AddLength();

	void Move(Vector3 arg_movePos, float arg_angle);

	DirectX::XMVECTOR AngleToVector2(float arg_angle);

	Vector3 GetStartPos();

	Vector3 GetEndPos();

	Vector3 GetVelocity();

private:

	OBJModel lineBox;

	Vector3 endPos;

	Vector3 velocity;   //startPosからendPosの向き　

	float angle;		//0で真横の線の予定

	float length;		//スケールの値

	double PI = 3.14159265f;
};

