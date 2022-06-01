#include "TestTriangle.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestTriangle::baseInfo = std::vector<LocusPointInfo>();

TestTriangle::TestTriangle(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
	:BaseLocus(arg_angle, arg_color)
{
	position = arg_pos;
	if (baseInfo.empty())
	{
		PointSetting();
	}

	ObjectManager* oManager = ObjectManager::GetInstance();

	//�����ł���������W�A�p�x�ɕϊ�����Line�𐶐�
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	for (int i = 0; i < baseInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(baseInfo[i].startPos.ConvertXMVECTOR(), rotMat);
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length, arg_color, Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}

TestTriangle::TestTriangle(const TestTriangle& arg_testTriangle, const DirectX::XMFLOAT4& arg_color)
	:TestTriangle(arg_testTriangle.position, arg_testTriangle.angle, arg_color)
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

void TestTriangle::Move(const Vector3& arg_movePos, const float arg_angle)
{
	position = arg_movePos;
	angle = arg_angle;
	//�����ł���������W�A�p�x�ɕϊ�����Line�𐶐�
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
	//�}�`���\��������W�@�I�_�܂�
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(0.8660f, 0.0f, 0.5f));
	points.push_back(Vector3(0.8660f, 0.0f, -0.5f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 12.0f;
	}

	CalcBaseInfo(points, baseInfo);
}