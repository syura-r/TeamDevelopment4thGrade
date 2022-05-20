#pragma once
#include "Object.h"
#include "OBJModel.h"

class Line : public Object
{
public:

	Line(Vector3 arg_startPos, float arg_angle, float arg_length, Vector4 arg_color, const Vector3& arg_scale);

	~Line();
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Reset();

	void AddLength(float arg_addSpeed);

	void Move(Vector3 arg_movePos, float arg_angle);

	Vector3 GetStartPos();

	Vector3 GetEndPos();

	Vector3 GetVelocity();

	float GetLength()const;

	void SetLength(const float arg_length);

	float GetAngle()const;

	bool IsDraw()const;
	void ChangeIsDraw(const bool arg_isDraw);

private:

	OBJModel lineBox;

	Vector3 endPos;

	Vector3 velocity;   //startPosからendPosの向き　

	float angle;		//0で真横の線の予定

	float length;		//スケールの値

	bool isDraw;
};

