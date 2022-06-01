#include "TestHexagram.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestHexagram::baseInfo = std::vector<LocusPointInfo>();

TestHexagram::TestHexagram(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
	:BaseLocus(arg_angle, arg_color)
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
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length, arg_color, Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}
//Vector4(1, 1, 0, 0.6f)

TestHexagram::TestHexagram(const TestHexagram& arg_testHexagram, const DirectX::XMFLOAT4& arg_color)
	:TestHexagram(arg_testHexagram.position, arg_testHexagram.angle, arg_color)
{
}

TestHexagram::~TestHexagram()
{
}

void TestHexagram::Initialize()
{
}

void TestHexagram::Update()
{
}

void TestHexagram::DrawReady()
{
}

void TestHexagram::Draw()
{
}

void TestHexagram::Move(const Vector3& arg_movePos, const float arg_angle)
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

const LocusType TestHexagram::GetType() const
{
	return LocusType::HEXAGRAM;
}

void TestHexagram::PointSetting()
{
	//図形を構成する座標　終点まで
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(0.8333f, 0.0f, 0.4166f));
	points.push_back(Vector3(0.1666f, 0.0f, -0.4166f));
	points.push_back(Vector3(1.0f, 0.0f, 0.0f));
	points.push_back(Vector3(0.1666f, 0.0f, 0.4166f));
	points.push_back(Vector3(0.8333f, 0.0f, -0.4166f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 16.0f;
	}

	CalcBaseInfo(points, baseInfo);
}