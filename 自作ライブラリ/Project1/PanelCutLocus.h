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
	void RecordCuttedPanelPos();

	int GetMaxNumLine()override;
	int GetCutPower()const;
	void SetCutPower(const int arg_power);
	std::vector<Vector2>& GetCuttedPanelPos();
	Object* GetParentObject();
	void SetParentObject(Object* arg_obj);

private:
	int cutPower;
	std::vector<Vector2> CuttedPanelPos;
	Object* parentObject;

	//���̐}�`�̃f�t�H���g�̏��
	static std::unordered_map<int, std::vector<LocusPointInfo>> baseInfo;
	static std::unordered_map<int, std::vector<Vector2>> baseCuttedDir;

	//�f�t�H���g�̐}�`������񂾂��v�Z
	void PointSetting()override;
};