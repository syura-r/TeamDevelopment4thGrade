#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class TestHexagram : public virtual Object, public virtual BaseLocus
{
public:
	TestHexagram(const Vector3& arg_pos, const float arg_angle);
	//�R�s�[�R���X�g���N�^
	TestHexagram(const TestHexagram& arg_testHexagram);
	~TestHexagram();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	void Move(const Vector3 arg_movePos, const float arg_angle)override;
	const LocusType GetType()const override;

private:
	//���̐}�`�̃f�t�H���g�̏��
	static std::vector<LocusPointInfo> baseInfo;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting()override;
};