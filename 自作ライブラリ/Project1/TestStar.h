#pragma once
#include "Line.h"
#include "LocusDef.h"

class TestStar : public Object
{
public:
	TestStar(const Vector3& arg_pos, const float arg_angle);
	~TestStar();

	void Initialize()override;
	void Update()override;
	void Draw()override;


private:
	//���̐}�`�̃f�t�H���g�̏��
	static std::vector<LocusPointInfo> baseInfo;

	Vector3 position;
	float angle;
	std::vector<Line*> lines;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting();
};