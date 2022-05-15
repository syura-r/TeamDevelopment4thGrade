#include "TestStar.h"
std::vector<LocusPointInfo> TestStar::baseInfo = std::vector<LocusPointInfo>();

TestStar::TestStar(const Vector3& arg_pos, const float angle)
{
	if (baseInfo.empty())
	{
		PointSetting();
	}
}

TestStar::~TestStar()
{
}

void TestStar::Initialize()
{
}

void TestStar::Update()
{
}

void TestStar::Draw()
{
}

void TestStar::PointSetting()
{
}
