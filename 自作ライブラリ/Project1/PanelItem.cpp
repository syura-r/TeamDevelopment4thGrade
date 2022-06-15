#include "PanelItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"

const float PanelItem::RADIUS = 1.5f;

PanelItem::PanelItem(const Vector3& arg_position)
	:virtualityPlanePosition(arg_position)
{
	Create(OBJLoader::GetModel("sphere"));	

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("PanelItem", this);

	Initialize();
}

PanelItem::~PanelItem()
{	
	ActorManager::GetInstance()->DeleteObject(this);
}

void PanelItem::Initialize()
{
	name = typeid(*this).name();
	position = virtualityPlanePosition;
	scale = Vector3(RADIUS, RADIUS, RADIUS);
	color = { 0.5f, 1.0f, 0.4f, 1.0f };	
	Object::Update();
}

void PanelItem::Update()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), Vector3(0, -5, 0));

	Object::Update();
}

Vector3 PanelItem::GetVirtualityPlanePosition()
{
	return virtualityPlanePosition;
}

float PanelItem::GetRadius()
{
	return RADIUS;
}
