#include "ItemEmitter.h"
#include "ObjectManager.h"
#include "EnergyItem.h"
#include "Field.h"
#include "ActorManager.h"

ItemEmitter* ItemEmitter::instance = nullptr;

ItemEmitter::ItemEmitter()
	:energyItemTimer(new Timer(300))
{
}

ItemEmitter::~ItemEmitter()
{
	delete energyItemTimer;
}

ItemEmitter* ItemEmitter::GetInstance()
{
	if (!instance)
	{
		instance = new ItemEmitter();
	}
	return instance;
}

void ItemEmitter::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void ItemEmitter::Initialize()
{
	energyItemTimer->Reset();
}

void ItemEmitter::Update()
{
	energyItemTimer->Update();
	if (energyItemTimer->IsTime())
	{
		EmitEnergyItem(GetRandomEmitPosition(3, 38));
	}
}

void ItemEmitter::EmitEnergyItem(const Vector3& arg_pos)
{
	EnergyItem* item = new EnergyItem(arg_pos);
	ObjectManager::GetInstance()->Add(item);	
	energyItemTimer->Reset();
}

Vector3 ItemEmitter::GetRandomEmitPosition(const int arg_min, const int arg_max)
{
	Vector3 returnVal;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	
	returnVal.y = field->GetPosition().y + EnergyItem::GetRadius();
	float angle = rand() % 360;
	float range = rand() % (arg_max - arg_min) + arg_min;
	returnVal += LocusUtility::AngleToVector2(angle) * range;

	return returnVal;
}
