#include "EnergyItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"

const float EnergyItem::RADIUS = 1.0f;

EnergyItem::EnergyItem(const Vector3& arg_position)
	:virtualityPlanePosition(arg_position)
{
	Create(OBJLoader::GetModel("sphere"));

	appearTimer = new Timer(120);

	//ActorManager::GetInstance()->AddObject("item", this);

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
	scale = Vector3(0.0001f, 0.0001f, 0.0001f);
	color = { 0.8f, 0.8f, 0.8f, 1.0f };
	appearTimer->Initialize();
	Object::Update();
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

bool EnergyItem::IsAppeared()
{
	return appearTimer->IsTime();
}
