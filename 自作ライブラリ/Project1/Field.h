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

	//�}�`�����ɂ���ČX���Ƃ�
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();

	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;

	//�L��
	static const Vector2 LOWER_LIMIT;
	static const Vector2 UPPER_LIMIT;

private:
	//�L��
	const float RADIUS = 45.0f;
	//�X����\��
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//�ǂ̕����ɌX���Ă��邩
	Vector3 angleTilt;
	Vector3 localYvec;
	//�}�`�̏ꏊ�Əd��
	std::vector<LocusFieldInfluence> influences;

	//�X���v�Z
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
};