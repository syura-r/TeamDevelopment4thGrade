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
	mapGameObject.insert(std::make_pair(arg_name, arg_object));
}

void ActorManager::DeleteObject(Object* arg_object)
{
	for (auto itr = mapGameObject.begin(); itr != mapGameObject.end(); itr++)
	{
		if (itr->second == arg_object)
		{
			mapGameObject.erase(itr);
			return;
		}
	}
}

Player* ActorManager::GetPlayer()
{
	auto range = mapGameObject.equal_range("Player");
	if (range.first != range.second)
	{
		Player* player = static_cast<Player*>(range.first->second);
		return player;
	}
	return nullptr;
}

std::vector<Field*>& ActorManager::GetFields()
{
	static std::vector<Field*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("Field");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<Field*>(itr->second));
	}

	return vec;
}

std::vector<StandardEnemy*>& ActorManager::GetStandardEnemies()
{
	static std::vector<StandardEnemy*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("StandardEnemy");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<StandardEnemy*>(itr->second));
	}

	return vec;
}

std::vector<EnergyItem*>& ActorManager::GetEnergyItems()
{
	static std::vector<EnergyItem*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("EnergyItem");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<EnergyItem*>(itr->second));
	}

	return vec;
}
