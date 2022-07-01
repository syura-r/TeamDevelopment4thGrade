#include "PanelCutLocus.h"
#include "ObjectManager.h"
#include "DrawMode.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"
std::unordered_map<int, std::vector<LocusPointInfo>> PanelCutLocus::baseInfo = std::unordered_map<int, std::vector<LocusPointInfo>>();
std::unordered_map<int, std::vector<Vector2>> PanelCutLocus::baseCuttedDir = std::unordered_map<int, std::vector<Vector2>>();

PanelCutLocus::PanelCutLocus(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
	:BaseLocus(arg_angle, arg_color),
	 cutPower(0),
	 parentObject(nullptr)
{
	virtualityPlanePosition = arg_pos;
	position = virtualityPlanePosition;
	size = 12.0f;
	weight = 10.0f;
	if (baseInfo.empty())
	{
		PointSetting();
	}
	CuttedPanelPos.clear();

	ObjectManager* oManager = ObjectManager::GetInstance();

	for (int i = 0; i < 7; i++)
	{
		Line* line = new Line(Vector3(), 0, FieldPiece::GetSidewaysLength() * 2, arg_color, Vector3(0.5f, 0.5f, 0.5f));
		lines.push_back(line);
		oManager->Add(line, true);
	}

	//引数でもらった座標、角度に変換してLineを生成
	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	auto vecInfo = baseInfo[5];
	for (int i = 0; i < vecInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(vecInfo[i].startPos.ConvertXMVECTOR(), rotMat);
		lines[i]->Move(rotatedPos + virtualityPlanePosition, angle + vecInfo[i].angle);
	}

	name = typeid(*this).name();	
}

PanelCutLocus::PanelCutLocus(const PanelCutLocus& arg_panelCut, const DirectX::XMFLOAT4& arg_color)
	:PanelCutLocus(arg_panelCut.virtualityPlanePosition, arg_panelCut.angle, arg_color)
{
}

PanelCutLocus::~PanelCutLocus()
{
}

void PanelCutLocus::Initialize()
{
}

void PanelCutLocus::Update()
{
}

void PanelCutLocus::DrawReady()
{
}

void PanelCutLocus::Draw()
{
}

void PanelCutLocus::Move(const Vector3& arg_movePos, const float arg_angle)
{
	virtualityPlanePosition = arg_movePos;
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	angle = arg_angle;
	CuttingInfo* info = ActorManager::GetInstance()->GetFields()[0]->GetCuttingInfo(parentObject);
	//引数でもらった座標、角度に変換してLineを生成
	if (!info->ridingPiece)
	{
		for (auto l : lines)
		{
			l->ChangeIsDraw(false);
		}
	}
	else if (cutPower != 0)
	{
		XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
		auto vecInfo = baseInfo[cutPower];
		int i = 0;
		for (i = 0; i < vecInfo.size(); i++)
		{
			Vector3 rotatedPos = CalcPointTransform(vecInfo[i].startPos.ConvertXMVECTOR(), rotMat);
			lines[i]->SetLength(vecInfo[i].length);
			lines[i]->Move(rotatedPos + virtualityPlanePosition, angle + vecInfo[i].angle);
			lines[i]->ChangeIsDraw(true);
		}

		for (; i < lines.size(); i++)
		{
			lines[i]->ChangeIsDraw(false);
		}
	}
	else
	{
		for (auto l : lines)
		{
			l->ChangeIsDraw(false);
		}
	}
}

const LocusType PanelCutLocus::GetType() const
{
	return LocusType::PANELCUT;
}

void PanelCutLocus::RecordCuttedPanelPos()
{
	CuttedPanelPos.clear();

	XMMATRIX rotMat = XMMatrixRotationY(XMConvertToRadians(angle));
	auto vecInfo = baseCuttedDir[cutPower];
	int i = 0;
	for (i = 0; i < vecInfo.size(); i++)
	{
		Vector3 rotatedPos = CalcPointTransform(LocusUtility::Dim2XZToDim3(vecInfo[i], -5).ConvertXMVECTOR(), rotMat);	
		CuttedPanelPos.push_back(LocusUtility::Dim3ToDim2XZ(rotatedPos + virtualityPlanePosition));
	}
}

int PanelCutLocus::GetMaxNumLine()
{
	return baseInfo[cutPower].size();
}

int PanelCutLocus::GetCutPower() const
{
	return cutPower;
}

void PanelCutLocus::SetCutPower(const int arg_power)
{
	cutPower = arg_power;
	Move(virtualityPlanePosition, angle);
}

std::vector<Vector2>& PanelCutLocus::GetCuttedPanelPos()
{
	return CuttedPanelPos;
}

Object* PanelCutLocus::GetParentObject()
{
	return parentObject;
}

void PanelCutLocus::SetParentObject(Object* arg_obj)
{
	parentObject = arg_obj;
}

void PanelCutLocus::PointSetting()
{
	const float sidewaysLength = FieldPiece::GetSidewaysLength();
	const float verticalLength = FieldPiece::GetFullOffset();

	std::vector<Vector3> points;
	std::vector<Vector2> cuttedDir;
	//1
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	baseInfo.emplace(1, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[1]);

	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	baseCuttedDir.emplace(1, cuttedDir);

	points.clear();
	cuttedDir.clear();
	//2
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	baseInfo.emplace(2, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[2]);

	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 0.5f));
	baseCuttedDir.emplace(2, cuttedDir);

	points.clear();
	cuttedDir.clear();
	//3
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(-sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	baseInfo.emplace(3, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[3]);

	cuttedDir.push_back(Vector2(0.0f, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 0.5f));
	baseCuttedDir.emplace(3, cuttedDir);

	points.clear();
	cuttedDir.clear();
	//4
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(-sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, verticalLength * 2));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)	
	baseInfo.emplace(4, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[4]);

	cuttedDir.push_back(Vector2(0.0f, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 1.5f));
	baseCuttedDir.emplace(4, cuttedDir);

	points.clear();
	cuttedDir.clear();
	//5
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 4, 0.0f, 0.0f));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, -verticalLength));
	points.push_back(Vector3(sidewaysLength, 0.0f, -verticalLength));
	points.push_back(Vector3(0.0f, 0.0f, 0.0));
	baseInfo.emplace(5, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[5]);

	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength, -verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, -verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 3, -verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 3, verticalLength * 0.5f));
	baseCuttedDir.emplace(5, cuttedDir);

	points.clear();
	cuttedDir.clear();
	//6
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	points.push_back(Vector3(-sidewaysLength, 0.0f, verticalLength));
	points.push_back(Vector3(0.0f, 0.0f, verticalLength * 2));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, verticalLength * 2));
	points.push_back(Vector3(sidewaysLength * 3, 0.0f, verticalLength));
	points.push_back(Vector3(sidewaysLength * 2, 0.0f, 0.0f));
	points.push_back(Vector3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < points.size(); i++)	
	baseInfo.emplace(6, std::vector<LocusPointInfo>());
	CalcBaseInfo(points, baseInfo[6]);

	cuttedDir.push_back(Vector2(0, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(0, verticalLength * 1.5f));
	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength, verticalLength * 1.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 0.5f));
	cuttedDir.push_back(Vector2(sidewaysLength * 2, verticalLength * 1.5f));
	baseCuttedDir.emplace(6, cuttedDir);

	points.clear();
	cuttedDir.clear();
}
