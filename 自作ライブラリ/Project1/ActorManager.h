#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"

class Player;
class TestBoss;
class Field;
class StandardEnemy;
class EnergyItem;
class PanelItem;

class ActorManager
{
public:
	static ActorManager* GetInstance();
	~ActorManager();

	void Initialize();

	void AddObject(std::string arg_name, Object* arg_object);
	void DeleteObject(Object* arg_object);	

	Player* GetPlayer();	
	std::vector<Field*>& GetFields();
	std::vector<StandardEnemy*>& GetStandardEnemies();
	std::vector<EnergyItem*>& GetEnergyItems();
	std::vector<PanelItem*>& GetPanelItems();

private:
	std::unordered_multimap<std::string, Object*> mapGameObject;
};

