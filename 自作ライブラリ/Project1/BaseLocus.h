#pragma once
#include "Line.h"
#include "LocusDef.h"

class BaseLocus
{
public:
	BaseLocus(const float arg_angle);
	virtual ~BaseLocus();

	virtual void Move(const Vector3 arg_movePos, const float arg_angle) = 0;
	virtual Line* GetLine(const int arg_num);
	virtual int GetMaxNumLine();

protected:
	float angle;
	std::vector<Line*> lines;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	virtual void PointSetting() = 0;
	//���W�̕ϊ��v�Z�p
	virtual const Vector3 CalcPointTransform(const DirectX::XMVECTOR& arg_point, const DirectX::XMMATRIX& arg_rotMat);
};