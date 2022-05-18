#include "BaseLocus.h"
#include "ObjectManager.h"
#include "DrawMode.h"

BaseLocus::BaseLocus(const float arg_angle)
	:angle(arg_angle)
{
	lines.clear();
}

BaseLocus::BaseLocus(const BaseLocus& arg_baseLocus)
	:BaseLocus(arg_baseLocus.angle)
{
}

BaseLocus::~BaseLocus()
{
	ObjectManager* oManager = ObjectManager::GetInstance();
	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i])
		{
			oManager->Remove(lines[i]);
		}
	}
}

Line* BaseLocus::GetLine(const int arg_num)
{
	return lines[arg_num];
}

int BaseLocus::GetMaxNumLine()
{
	return lines.size();
}

void BaseLocus::CalcBaseInfo(const std::vector<Vector3>& arg_points, std::vector<LocusPointInfo>& arg_infoList)
{
	const double PI = 3.14159265f;

	//ÉèÅ[ÉãÉhXé≤Ç∆ÇÃÇ»Ç∑äpÇÃåvéZ
	float angle = 0;
	Vector3 vecX = Vector3(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < arg_points.size() - 1; i++)
	{
		angle = 0;
		Vector3 line = arg_points[i + 1] - arg_points[i];

		arg_infoList.push_back(LocusPointInfo());
		arg_infoList[i].startPos = arg_points[i];
		arg_infoList[i].endPos = arg_points[i + 1];
		arg_infoList[i].length = Vector3::Length(line);

		float cos = vecX.Dot(line) / (vecX.Length() * line.Length());
		angle = acosf(cos) * 180.0f / PI;
		Vector3 cross = vecX.Cross(line);
		if (cross.y < 0)
		{
			angle = 360.0f - angle;
		}
		arg_infoList[i].angle = angle;
	}
}

const Vector3 BaseLocus::CalcPointTransform(const DirectX::XMVECTOR& arg_point, const DirectX::XMMATRIX& arg_rotMat)
{	
	XMMATRIX posMat = XMMATRIX(arg_point, XMVECTOR(), XMVECTOR(), XMVECTOR());
	posMat *= arg_rotMat;
	return Vector3(posMat.r[0].m128_f32[0], posMat.r[0].m128_f32[1], posMat.r[0].m128_f32[2]);
}
