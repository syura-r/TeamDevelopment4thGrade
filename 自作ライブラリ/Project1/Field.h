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
	//L‚³
	const float RADIUS = 45.0f;
	//ŒX‚«‚ğ•\‚·
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//‚Ç‚Ì•ûŒü‚ÉŒX‚¢‚Ä‚¢‚é‚©
	Vector3 vecTilt;

	//ŒX‚«ŒvZ
	void CalcTilt();
};