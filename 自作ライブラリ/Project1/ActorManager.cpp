#include "ActorManager.h"
#include "Player.h"
#include "TestBoss.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "UnableThroughBlock.h"
#include "UnableThroughEdge.h"
#include "CircularSaw.h"

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

std::vector<PanelItem*>& ActorManager::GetPanelItems()
{
	static std::vector<PanelItem*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("PanelItem");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<PanelItem*>(itr->second));
	}

	return vec;
}

std::vector<UnableThroughBlock*>& ActorManager::GetUnableThroughBlocks()
{
	static std::vector<UnableThroughBlock*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("UnableThroughBlock");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<UnableThroughBlock*>(itr->second));
	}

	return vec;
}

std::vector<UnableThroughEdge*>& ActorManager::GetUnableThroughEdges()
{
	static std::vector<UnableThroughEdge*> vec;
	vec.clear();

	auto range = mapGameObject.equal_range("UnableThroughEdge");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		vec.push_back(static_cast<UnableThroughEdge*>(itr->second));
	}

	return vec;
}

CircularSaw* ActorManager::GetCircularSaw(Object* arg_obj)
{
	CircularSaw* returnPointer = nullptr;
	auto range = mapGameObject.equal_range("CircularSaw");
	for (auto itr = range.first; itr != range.second; itr++)
	{
		CircularSaw* c = static_cast<CircularSaw*>(itr->second);
		if (c->GetParentObject() == arg_obj)
		{
			returnPointer = c;
			break;
		}
	}
	return returnPointer;
}
