#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"

class Field : public virtual Object
{
public:
	Field();
	~Field();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;

private:
	//�L��
	const float RADIUS = 45.0f;
	//�X����\��
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//�ǂ̕����ɌX���Ă��邩
	Vector3 vecTilt;

	//�X���v�Z
	void CalcTilt();
};