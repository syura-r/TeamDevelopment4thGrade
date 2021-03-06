#pragma once
#include "Object.h"
#include "OBJModel.h"

class Line : public Object
{
public:

	Line(const Vector3& arg_startPos, float arg_angle, float arg_length, const DirectX::XMFLOAT4& arg_color, const Vector3& arg_scale);
	~Line();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	//描き足し
	void AddLength(float arg_addSpeed);
	//図形の一部としての移動
	void Move(Vector3 arg_movePos, float arg_angle);

	Vector3 GetStartPos();
	Vector3 GetEndPos();
	Vector3 GetDirection();
	float GetLength()const;
	void SetLength(const float arg_length);
	float GetAngle()const;
	bool IsDraw()const;
	void ChangeIsDraw(const bool arg_isDraw);
	Vector3 GetVirtualityPlaneStartPos()const;
	Vector3 GetVirtualityPlaneEndPos()const;
	Vector3 GetCenter()const;

private:
	Vector3 endPos;
	Vector3 direction;
	float angle;
	float length;
	//描画するか
	bool isDraw;
	Vector3 virtualityPlaneStartPos;
	Vector3 virtualityPlaneEndPos;

	void UpdateRotation();
};

