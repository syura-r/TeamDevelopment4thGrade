#include "TestTriangle.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestTriangle::baseInfo = std::vector<LocusPointInfo>();

TestTriangle::TestTriangle(const Vector3& arg_pos, const float arg_angle)
	:BaseLocus(arg_angle)
{
	position = arg_pos;
	if (baseInfo.empty())
	{
		PointSetting();
	}

	ObjectManager* oManager = ObjectManager::GetInstance();

	//引数でもらった座標、角度に変換してLineを生成
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	for (int i = 0; i < baseInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(baseInfo[i].startPos.ConvertXMVECTOR(), rotMat);
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length, Vector4(1, 1, 0, 0.3f), Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}

TestTriangle::TestTriangle(const TestTriangle& arg_testTriangle)
	:TestTriangle(arg_testTriangle.position, arg_testTriangle.angle)
{
}

TestTriangle::~TestTriangle()
{
}

void TestTriangle::Initialize()
{
}

void TestTriangle::Update()
{
}

void TestTriangle::DrawReady()
{
}

void TestTriangle::Draw()
{
}

void TestTriangle::Move(const Vector3 arg_movePos, const float arg_angle)
{
	position = arg_movePos;
	angle = arg_angle;
	//引数でもらった座標、角度に変換してLineを生成
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	for (int i = 0; i < baseInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(baseInfo[i].startPos.ConvertXMVECTOR(), rotMat);
		lines[i]->Move(rotatedPos + position, angle + baseInfo[i].angle);
	}
}

const LocusType TestTriangle::GetType() const
{
	return LocusType::TRIANGLE;
}

void TestTriangle::PointSetting()
{
	//図形を構成する座標　終点まで
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(5.1961f, 0.0f, 3.0f));
	points.push_back(Vector3(5.1961f, 0.0f, -3.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 2.0f;
	}

	CalcBaseInfo(points, baseInfo);
}