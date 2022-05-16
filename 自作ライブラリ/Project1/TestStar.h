#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class TestStar : public Object, public BaseLocus
{
public:
	TestStar(const Vector3& arg_pos, const float arg_angle);
	~TestStar();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	void Move(const Vector3 arg_movePos, const float arg_angle)override;	

private:
	//���̐}�`�̃f�t�H���g�̏��
	static std::vector<LocusPointInfo> baseInfo;	

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting()override;
};