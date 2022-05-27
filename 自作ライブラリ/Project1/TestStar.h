#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class TestStar : public virtual Object, public virtual BaseLocus
{
public:
	TestStar(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color);
	//�R�s�[�R���X�g���N�^
	TestStar(const TestStar& arg_testStar, const DirectX::XMFLOAT4& arg_color);
	~TestStar();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	void Move(const Vector3& arg_movePos, const float arg_angle)override;
	const LocusType GetType()const override;

private:
	//���̐}�`�̃f�t�H���g�̏��
	static std::vector<LocusPointInfo> baseInfo;	

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting()override;
};