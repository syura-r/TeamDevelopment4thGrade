#pragma once
#include "Line.h"
#include "LocusDef.h"

class BaseLocus : public virtual Object
{
public:
	BaseLocus(const float arg_angle, const DirectX::XMFLOAT4& arg_color);
	BaseLocus(const BaseLocus& arg_baseLocus);
	virtual ~BaseLocus();

	void ChangeColor(const Vector4& arg_color);

	virtual void Move(const Vector3& arg_movePos, const float arg_angle) = 0;
	virtual const LocusType GetType()const = 0;
	float GetAngle()const;
	Line* GetLine(const int arg_num);
	virtual int GetMaxNumLine();
	bool IsDraw()const;
	void ChangeIsDraw(const bool arg_isDraw);
	float GetWeight()const;
	//�d�S
	virtual const Vector3 GetCenterOfGravity();
	Vector3 GetVirtualityPlanePosition()const;

protected:
	float angle;
	std::vector<Line*> lines;
	bool isDraw;
	float size;
	float weight;
	Vector3 virtualityPlanePosition;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	virtual void PointSetting() = 0;
	void CalcBaseInfo(const std::vector<Vector3>& arg_points, std::vector<LocusPointInfo>& arg_infoList);
	//���W�̕ϊ��v�Z�p
	const Vector3 CalcPointTransform(const DirectX::XMVECTOR& arg_point, const DirectX::XMMATRIX& arg_rotMat);
};