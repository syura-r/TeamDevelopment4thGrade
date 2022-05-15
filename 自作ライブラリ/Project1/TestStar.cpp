#include "TestStar.h"
#include "ObjectManager.h"
std::vector<LocusPointInfo> TestStar::baseInfo = std::vector<LocusPointInfo>();

TestStar::TestStar(const Vector3& arg_pos, const float arg_angle)
	:position(arg_pos),
	 angle(arg_angle)
{
	if (baseInfo.empty())
	{
		PointSetting();
	}

	ObjectManager* oManager = ObjectManager::GetInstance();

	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));

	for (int i = 0; i < baseInfo.size(); i++)
	{
		XMVECTOR pos = baseInfo[i].startPos.ConvertXMVECTOR();
		XMMATRIX posMat = XMMATRIX(pos, XMVECTOR(), XMVECTOR(), XMVECTOR());
		posMat *= rotMat;
		Vector3 rotatedPos;
		rotatedPos.x = posMat.r[0].m128_f32[0];
		rotatedPos.y = posMat.r[0].m128_f32[1];
		rotatedPos.z = posMat.r[0].m128_f32[2];
		Line* line = new Line(rotatedPos + position, angle + baseInfo[i].angle, baseInfo[i].length * 0.25f, Vector4(1, 1, 0, 1));
		lines.push_back(line);
		oManager->Add(line, false);
	}
}

TestStar::~TestStar()
{
	int a = 5;
	a++;
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
	const double PI = 3.14159265f;

	//図形を構成する座標　終点まで
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(1.8090f, 0.0f, 0.5878f));
	points.push_back(Vector3(0.6910f, 0.0f, -0.9511f));
	points.push_back(Vector3(0.6910f, 0.0f, 0.9511f));
	points.push_back(Vector3(1.8090f, 0.0f, -0.5878f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 10.0f;
	}

	float angle = 0;
	Vector3 vecX = Vector3(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < points.size() - 1; i++)
	{
		angle = 0;
		Vector3 line = points[i + 1] - points[i];

		baseInfo.push_back(LocusPointInfo());
		baseInfo[i].startPos = points[i];
		baseInfo[i].length = Vector3::Length(line);

		float cos = vecX.Dot(line) / (vecX.Length() * line.Length());
		angle = acosf(cos) * 180.0f / PI;
		Vector3 cross = vecX.Cross(line);
		if (cross.y < 0)
		{
			angle = 360.0f - angle;
		}
		baseInfo[i].angle = angle;
	}
}
