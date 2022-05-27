#include "TestTriforce.h"
#include "ObjectManager.h"
#include "DrawMode.h"
std::vector<LocusPointInfo> TestTriforce::baseInfo = std::vector<LocusPointInfo>();

TestTriforce::TestTriforce(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
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

TestTriforce::TestTriforce(const TestTriforce& arg_testTriforce, const DirectX::XMFLOAT4& arg_color)
	:TestTriforce(arg_testTriforce.position, arg_testTriforce.angle, arg_color)
{
}

TestTriforce::~TestTriforce()
{
}

void TestTriforce::Initialize()
{
}

void TestTriforce::Update()
{
}

void TestTriforce::DrawReady()
{
//#ifdef _DEBUG
//	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
//	{
//		ImGui::Begin("TestStar");
//		ImGui::Text("[0].startPos : {%f, %f, %f }\n", lines[0]->GetStartPos().x, lines[0]->GetStartPos().y, lines[0]->GetStartPos().z);
//		ImGui::Text("[0].velocity : {%f, %f, %f }\n", lines[0]->GetVelocity().x, lines[0]->GetVelocity().y, lines[0]->GetVelocity().z);
//		ImGui::Text("[0].endPos : {%f, %f, %f }\n", lines[0]->GetEndPos().x, lines[0]->GetEndPos().y, lines[0]->GetEndPos().z);
//		ImGui::Text("[0].angle : {%f}\n", lines[0]->GetAngle());
//		ImGui::End();
//	}
//#endif
//
//	if (Object3D::GetDrawShadow())
//		pipelineName = "ShadowMap";
//	else
//	{
//		pipelineName = "DrawShadowOBJ";
//	}
}

void TestTriforce::Draw()
{
}

void TestTriforce::Move(const Vector3& arg_movePos, const float arg_angle)
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

const LocusType TestTriforce::GetType() const
{
	return LocusType::TRIFORCE;
}

void TestTriforce::PointSetting()
{
	//図形を構成する座標　終点まで
	std::vector<Vector3> points;
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(3.464f, 0.0f, 2.0f));
	points.push_back(Vector3(3.464f, 0.0f, -2.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 4.0f));
	points.push_back(Vector3(6.928f, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, -4.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));	
	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 2.0f;
	}

	CalcBaseInfo(points, baseInfo);
}