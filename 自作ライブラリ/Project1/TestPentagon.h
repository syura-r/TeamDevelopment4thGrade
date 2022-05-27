#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class TestPentagon : public virtual Object, public virtual BaseLocus
{
public:
	TestPentagon(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color);
	//�R�s�[�R���X�g���N�^
	TestPentagon(const TestPentagon& arg_testPentagon, const DirectX::XMFLOAT4& arg_color);
	~TestPentagon();

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