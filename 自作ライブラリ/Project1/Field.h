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

	//}Œ`Š®¬‚É‚æ‚Á‚ÄŒX‚­‚Æ‚«
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();

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
	//}Œ`‚ÌêŠ‚Æd‚³
	std::vector<LocusFieldInfluence> influences;

	//ŒX‚«ŒvZ
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
};