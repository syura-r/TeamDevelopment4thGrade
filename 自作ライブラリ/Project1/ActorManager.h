#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"

class Player;
class Field;
class StandardEnemy;
class EnergyItem;
class PanelItem;
class UnableThroughBlock;
class UnableThroughEdge;
class CircularSaw;
class BaseGameActor;

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
	std::vector<UnableThroughBlock*>& GetUnableThroughBlocks();
	std::vector<UnableThroughEdge*>& GetUnableThroughEdges();
	CircularSaw* GetCircularSaw(Object* arg_obj);

private:
	std::unordered_multimap<std::string, Object*> mapGameObject;
};

