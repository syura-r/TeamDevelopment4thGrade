#pragma once
#include "Line.h"
#include "LocusDef.h"
#include "BaseLocus.h"

class PanelCutLocus : public virtual Object, public virtual BaseLocus
{
public:
	PanelCutLocus(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color);
	//�R�s�[�R���X�g���N�^
	PanelCutLocus(const PanelCutLocus& arg_panelCut, const DirectX::XMFLOAT4& arg_color);
	~PanelCutLocus();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	void Move(const Vector3& arg_movePos, const float arg_angle)override;
	const LocusType GetType()const override;

private:
	//���̐}�`�̃f�t�H���g�̏��
	static std::vector<std::vector<LocusPointInfo>> baseInfo;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting()override;
};