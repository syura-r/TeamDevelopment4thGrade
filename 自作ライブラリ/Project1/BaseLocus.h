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

	//デフォルトの図形情報を一回だけ計算
	virtual void PointSetting() = 0;
};