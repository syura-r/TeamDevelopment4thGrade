#include "BaseLocus.h"
#include "ObjectManager.h"
#include "DrawMode.h"

BaseLocus::BaseLocus(const float arg_angle)
	:angle(arg_angle)
{
	lines.clear();
}

BaseLocus::~BaseLocus()
{
}

Line* BaseLocus::GetLine(const int arg_num)
{
	return lines[arg_num];
}

int BaseLocus::GetMaxNumLine()
{
	return lines.size();
}

const Vector3 BaseLocus::CalcPointTransform(const DirectX::XMVECTOR& arg_point, const DirectX::XMMATRIX& arg_rotMat)
{	
	XMMATRIX posMat = XMMATRIX(arg_point, XMVECTOR(), XMVECTOR(), XMVECTOR());
	posMat *= arg_rotMat;
	return Vector3(posMat.r[0].m128_f32[0], posMat.r[0].m128_f32[1], posMat.r[0].m128_f32[2]);
}
