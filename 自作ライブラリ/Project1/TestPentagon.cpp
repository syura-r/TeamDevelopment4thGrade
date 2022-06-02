#include "TestPentagon.h"
#include "ObjectManager.h"
#include "DrawMode.h"
#include "ActorManager.h"
#include "Field.h"
std::vector<LocusPointInfo> TestPentagon::baseInfo = std::vector<LocusPointInfo>();

TestPentagon::TestPentagon(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
	:BaseLocus(arg_angle, arg_color)
{
	virtualityPlanePosition = arg_pos;
	position = virtualityPlanePosition;
	size = 20.0f;
	weight = 10.0f;
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
		Line* line = new Line(rotatedPos + virtualityPlanePosition, angle + baseInfo[i].angle, baseInfo[i].length, arg_color, Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}

TestPentagon::TestPentagon(const TestPentagon& arg_testPentagon, const DirectX::XMFLOAT4& arg_color)
	:TestPentagon(arg_testPentagon.virtualityPlanePosition, arg_testPentagon.angle, arg_color)
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

void TestPentagon::Move(const Vector3& arg_movePos, const float arg_angle)
{
	virtualityPlanePosition = arg_movePos;
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetField()->GetAngleTilt(), Vector3(0, -5, 0));
	angle = arg_angle;
	//引数でもらった座標、角度に変換してLineを生成
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	for (int i = 0; i < baseInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(baseInfo[i].startPos.ConvertXMVECTOR(), rotMat);
		lines[i]->Move(rotatedPos + virtualityPlanePosition, angle + baseInfo[i].angle);
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
	points.push_back(Vector3(0.3455f, 0.0f, 0.4756f));
	points.push_back(Vector3(0.9045f, 0.0f, 0.2939f));
	points.push_back(Vector3(0.9045f, 0.0f, -0.2939f));
	points.push_back(Vector3(0.3455f, 0.0f, -0.4756f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= size;
	}

	CalcBaseInfo(points, baseInfo);
}