#include "ActorManager.h"
#include "Player.h"
#include "TestBoss.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"

ActorManager* ActorManager::GetInstance()
{
	static ActorManager* instance = new ActorManager();
	return instance;
}



ActorManager::~ActorManager()
{
	mapGameObject.clear();//登録してあるオブジェクトは別の場所でdeleteされる
}

void ActorManager::Initialize()
{
	mapGameObject.clear(); 
}

void ActorManager::AddObject(std::string arg_name, Object* arg_object)
{
	if (mapGameObject[arg_name] != nullptr)//既にその名前で作成されたオブジェクトがある場合は終了
		return;

	mapGameObject[arg_name] = arg_object;
}

Object* ActorManager::GetGameObject(std::string arg_name)
{
	return mapGameObject[arg_name];
}

Player* ActorManager::GetPlayer()
{
	auto itr = mapGameObject.find("player");
	if (itr != mapGameObject.end())
	{
		Player* player = static_cast<Player*>(itr->second);
		return player;
	}
	return nullptr;
}

TestBoss* ActorManager::GetBoss()
{
	auto itr = mapGameObject.find("boss");
	if (itr != mapGameObject.end())
	{
		TestBoss* boss = static_cast<TestBoss*>(itr->second);
		return boss;
	}
	return nullptr;
}

Field* ActorManager::GetField()
{
	auto itr = mapGameObject.find("field");
	if (itr != mapGameObject.end())
	{
		Field* field = static_cast<Field*>(itr->second);
		return field;
	}
	return nullptr;
}

StandardEnemy* ActorManager::GetStandardEnemy()
{
	auto itr = mapGameObject.find("enemy");
	if (itr != mapGameObject.end())
	{
		StandardEnemy* enemy = static_cast<StandardEnemy*>(itr->second);
		return enemy;
	}
	return nullptr;
}

std::vector<EnergyItem*>& ActorManager::GetEnergyItems()
{
	static std::vector<EnergyItem*> items;
	items.clear();
	auto itr = mapGameObject.find("item");
	if (itr != mapGameObject.end())
	{
		EnergyItem* item = static_cast<EnergyItem*>(itr->second);
		items.push_back(item);
	}
	return items;
}
