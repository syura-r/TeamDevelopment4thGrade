#include "PanelCutLocus.h"
#include "ObjectManager.h"
#include "DrawMode.h"
#include "ActorManager.h"
#include "Field.h"
std::vector<std::vector<LocusPointInfo>> PanelCutLocus::baseInfo = std::vector<std::vector<LocusPointInfo>>();

PanelCutLocus::PanelCutLocus(const Vector3& arg_pos, const float arg_angle, const DirectX::XMFLOAT4& arg_color)
	:BaseLocus(arg_angle, arg_color)
{
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
}

const LocusType PanelCutLocus::GetType() const
{
	return LocusType::PANELCUT;
}

void PanelCutLocus::PointSetting()
{
}
