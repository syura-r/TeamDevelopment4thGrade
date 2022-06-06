#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"
#include "LocusDef.h"

class Field : public Object
{
public:
	Field();
	~Field();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;

	//図形完成によって傾くとき
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();

	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;

private:
	//広さ
	const float RADIUS = 45.0f;
	//傾きを表す
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//どの方向に傾いているか
	Vector3 angleTilt;
	Vector3 localYvec;
	//図形の場所と重さ
	std::vector<LocusFieldInfluence> influences;

	//傾き計算
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
};