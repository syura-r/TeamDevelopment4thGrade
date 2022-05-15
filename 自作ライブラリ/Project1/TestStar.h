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

	void Move(const Vector3 arg_movePos, const float arg_angle);

	Line* GetLine(const int arg_num);

private:
	//この図形のデフォルトの情報
	static std::vector<LocusPointInfo> baseInfo;

	Vector3 position;
	float angle;
	std::vector<Line*> lines;

	//デフォルトの図形情報を一回だけ計算
	void PointSetting();
};