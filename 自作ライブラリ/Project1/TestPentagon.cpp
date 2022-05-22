#include "TestPentagon.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestPentagon::baseInfo = std::vector<LocusPointInfo>();

TestPentagon::TestPentagon(const Vector3& arg_pos, const float arg_angle)
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
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length, Vector4(1, 1, 0, 0.6f), Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}

TestPentagon::TestPentagon(const TestPentagon& arg_testPentagon)
	:TestPentagon(arg_testPentagon.position, arg_testPentagon.angle)
{
}

TestPentagon::~TestPentagon()
{
}

void TestPentagon::Initialize()
{
}

void TestPentagon::Update()
{
}

void TestPentagon::DrawReady()
{
}

void TestPentagon::Draw()
{
}

void TestPentagon::Move(const Vector3 arg_movePos, const float arg_angle)
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

const LocusType TestPentagon::GetType() const
{
	return LocusType::PENTAGON;
}

void TestPentagon::PointSetting()
{
	//図形を構成する座標　終点まで
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(0.6910f, 0.0f, 0.9511f));
	points.push_back(Vector3(1.8090f, 0.0f, 0.5878f));
	points.push_back(Vector3(1.8090f, 0.0f, -0.5878f));
	points.push_back(Vector3(0.6910f, 0.0f, -0.9511f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 10.0f;
	}

	CalcBaseInfo(points, baseInfo);
}