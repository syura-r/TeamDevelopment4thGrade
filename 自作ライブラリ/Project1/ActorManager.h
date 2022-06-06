#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"

class Player;
class TestBoss;
class Field;
class StandardEnemy;
class EnergyItem;

class ActorManager
{
public:
	static ActorManager* GetInstance();
	~ActorManager();

	void Initialize();

	void AddObject(std::string arg_name, Object* arg_object);
	void DeleteObject(Object* arg_object);

	Object* GetGameObject(std::string arg_name);

	Player* GetPlayer();
	TestBoss* GetBoss();
	Field* GetField();
	StandardEnemy* GetStandardEnemy();
	std::vector<EnergyItem*>& GetEnergyItems();

private:
	std::unordered_map<std::string, Object*> mapGameObject;
};

