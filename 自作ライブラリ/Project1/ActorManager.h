#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"
#include "ObjectRegistType.h"

class Player;
class Field;
class StandardEnemy;
class EnergyItem;
class PanelItem;
class UnableThroughBlock;
class UnableThroughEdge;
class CircularSaw;
class BaseGameActor;
class FeverInItem;

class ActorManager
{
public:
	static ActorManager* GetInstance();
	static void DeleteInstance();

	void Initialize();

	void AddObject(Object* arg_object, const ObjectRegistType arg_type);
	void DeleteObject(Object* arg_object, const ObjectRegistType arg_type);
	void CollisionCheck();

	Player* GetPlayer();	
	std::vector<Field*>& GetFields();
	std::vector<StandardEnemy*>& GetStandardEnemies();
	std::vector<EnergyItem*>& GetEnergyItems();
	std::vector<PanelItem*>& GetPanelItems();
	std::vector<UnableThroughBlock*>& GetUnableThroughBlocks();
	std::vector<UnableThroughEdge*>& GetUnableThroughEdges();
	CircularSaw* GetCircularSaw(Object* arg_obj);
	std::vector<BaseGameActor*>& GetBaseGameActors();
	std::vector<FeverInItem*>& GetFeverInItems();

private:
	static ActorManager* instance;
	ActorManager();
	~ActorManager();
	ActorManager(const ActorManager& another) = delete;
	ActorManager& operator=(const ActorManager& another) = delete;

	std::vector<Player*> players;
	std::vector<Field*> fields;
	std::vector<StandardEnemy*> standardEnemies;
	std::vector<EnergyItem*> energyItems;
	std::vector<PanelItem*> panelItems;
	std::vector<UnableThroughBlock*> unableThroughBlocks;
	std::vector<UnableThroughEdge*> unableThroughEdges;
	std::vector<CircularSaw*> circularSaws;
	std::vector<BaseGameActor*> baseGameActors;
	std::vector<FeverInItem*> feverInItems;
};

