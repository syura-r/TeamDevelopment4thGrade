#pragma once
#include "Line.h"
#include "LocusDef.h"

enum class LocusType
{
	UNDIFINED = -1,
	STAR,
	RIBBON,
	TRIFORCE,
};

class BaseLocus : public virtual Object
{
public:
	BaseLocus(const float arg_angle);
	BaseLocus(const BaseLocus& arg_baseLocus);
	virtual ~BaseLocus();

	virtual void Move(const Vector3 arg_movePos, const float arg_angle) = 0;
	virtual const LocusType GetType()const = 0;
	Line* GetLine(const int arg_num);
	int GetMaxNumLine();
	bool IsDraw()const;
	void ChangeIsDraw(const bool arg_isDraw);

protected:	
	float angle;
	std::vector<Line*> lines;
	bool isDraw;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	virtual void PointSetting() = 0;
	void CalcBaseInfo(const std::vector<Vector3>& arg_points, std::vector<LocusPointInfo>& arg_infoList);
	//���W�̕ϊ��v�Z�p
	const Vector3 CalcPointTransform(const DirectX::XMVECTOR& arg_point, const DirectX::XMMATRIX& arg_rotMat);
};