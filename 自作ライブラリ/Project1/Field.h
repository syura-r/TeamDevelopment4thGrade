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
	//広さ
	const float RADIUS = 45.0f;
	//傾きを表す
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//どの方向に傾いているか
	Vector3 vecTilt;

	//傾き計算
	void CalcTilt();
};