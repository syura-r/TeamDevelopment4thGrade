#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class TestTriangle : public virtual Object, public virtual BaseLocus
{
public:
	TestTriangle(const Vector3& arg_pos, const float arg_angle);
	//コピーコンストラクタ
	TestTriangle(const TestTriangle& arg_testTriangle);
	~TestTriangle();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	void Move(const Vector3 arg_movePos, const float arg_angle)override;
	const LocusType GetType()const override;

private:
	//この図形のデフォルトの情報
	static std::vector<LocusPointInfo> baseInfo;

	//デフォルトの図形情報を一回だけ計算
	void PointSetting()override;
};