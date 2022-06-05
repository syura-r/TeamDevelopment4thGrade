#include "EnergyItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"

EnergyItem::EnergyItem(const Vector3& arg_position)
	:virtualityPlanePosition(arg_position)
{
	Create(OBJLoader::GetModel("sphere"));

	appearTimer = new Timer(120);

	Initialize();
}

EnergyItem::~EnergyItem()
{
	delete appearTimer;
}

void EnergyItem::Initialize()
{
	name = typeid(*this).name();		
	position = virtualityPlanePosition;
	scale = Vector3(0, 0, 0);
	color = { 0.8f, 0.8f, 0.8f, 1.0f };
	appearTimer->Initialize();
}

void EnergyItem::Update()
{
	if (!appearTimer->IsTime())
	{
		Appear();
	}

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetField()->GetAngleTilt(), Vector3(0, -5, 0));

	Object::Update();
}

void EnergyItem::Appear()
{
	appearTimer->Update();

	scale.x = scale.y = scale.z = Easing::Lerp(0, RADIUS, appearTimer->GetRate());

	if (appearTimer->IsTime())
	{
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
}
