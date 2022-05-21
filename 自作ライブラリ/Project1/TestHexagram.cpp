#include "TestHexagram.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestHexagram::baseInfo = std::vector<LocusPointInfo>();

TestHexagram::TestHexagram(const Vector3& arg_pos, const float arg_angle)
	:BaseLocus(arg_angle)
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
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length, Vector4(1, 1, 0, 0.3f), Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}
}

TestHexagram::TestHexagram(const TestHexagram& arg_testHexagram)
	:TestHexagram(arg_testHexagram.position, arg_testHexagram.angle)
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

void TestHexagram::Move(const Vector3 arg_movePos, const float arg_angle)
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

const LocusType TestHexagram::GetType() const
{
	return LocusType::HEXAGRAM;
}

void TestHexagram::PointSetting()
{
	//�}�`���\��������W�@�I�_�܂�
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(6.6666f, 0.0f, 3.3333f));
	points.push_back(Vector3(1.3333f, 0.0f, -3.3333f));
	points.push_back(Vector3(8.0f, 0.0f, 0.0f));
	points.push_back(Vector3(1.3333f, 0.0f, 3.3333f));
	points.push_back(Vector3(6.6666f, 0.0f, -3.3333f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 2.0f;
	}

	CalcBaseInfo(points, baseInfo);
}