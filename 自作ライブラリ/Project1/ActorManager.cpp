#include "ActorManager.h"
#include "Player.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "UnableThroughBlock.h"
#include "UnableThroughEdge.h"
#include "CircularSaw.h"
#include "BaseGameActor.h"
#include "IActionState.h"
#include "FeverInItem.h"

ActorManager* ActorManager::instance = nullptr;

ActorManager* ActorManager::GetInstance()
{
	if (!instance)
	{
		instance = new ActorManager();
	}
	return instance;
}

void ActorManager::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

ActorManager::ActorManager()
{
	Initialize();
}

ActorManager::~ActorManager()
{
	Initialize();//登録してあるオブジェクトは別の場所でdeleteされる
}

void ActorManager::Initialize()
{
	players.clear();
	fields.clear();
	standardEnemies.clear();
	energyItems.clear();
	panelItems.clear();
	unableThroughBlocks.clear();
	unableThroughEdges.clear();
	circularSaws.clear();
	baseGameActors.clear();
	feverInItems.clear();
}

void ActorManager::AddObject(Object* arg_object, const ObjectRegistType arg_type)
{
	switch (arg_type)
	{
	case ObjectRegistType::PLAYER:
		players.push_back(static_cast<Player*>(arg_object));
		baseGameActors.push_back(static_cast<Player*>(arg_object));
		break;
	case ObjectRegistType::FIELD:
		fields.push_back(static_cast<Field*>(arg_object));
		break;
	case ObjectRegistType::STANDARD_ENEMY:
		standardEnemies.push_back(static_cast<StandardEnemy*>(arg_object));
		baseGameActors.push_back(static_cast<StandardEnemy*>(arg_object));
		break;
	case ObjectRegistType::ENERGY_ITEM:
		energyItems.push_back(static_cast<EnergyItem*>(arg_object));
		break;
	case ObjectRegistType::PANEL_ITEM:
		panelItems.push_back(static_cast<PanelItem*>(arg_object));
		break;
	case ObjectRegistType::UNABLETHROUGH_BLOCK:
		unableThroughBlocks.push_back(static_cast<UnableThroughBlock*>(arg_object));
		break;
	case ObjectRegistType::UNABLETHROUGH_EDGE:
		unableThroughEdges.push_back(static_cast<UnableThroughEdge*>(arg_object));
		break;
	case ObjectRegistType::CIRCULAR_SAW:
		circularSaws.push_back(static_cast<CircularSaw*>(arg_object));
		break;
	case ObjectRegistType::FEVERIN_ITEM:
		feverInItems.push_back(static_cast<FeverInItem*>(arg_object));
	default:
		break;
	}
}

void ActorManager::DeleteObject(Object* arg_object, const ObjectRegistType arg_type)
{
	switch (arg_type)
	{
	case ObjectRegistType::PLAYER:
		for (auto itr = players.begin(); itr != players.end(); itr++)
		{
			if (*itr == arg_object)
			{
				players.erase(itr);
				break;
			}
		}
		for (auto itr = baseGameActors.begin(); itr != baseGameActors.end(); itr++)
		{
			if (*itr == arg_object)
			{
				baseGameActors.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::FIELD:
		for (auto itr = fields.begin(); itr != fields.end(); itr++)
		{
			if (*itr == arg_object)
			{
				fields.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::STANDARD_ENEMY:
		for (auto itr = standardEnemies.begin(); itr != standardEnemies.end(); itr++)
		{
			if (*itr == arg_object)
			{
				standardEnemies.erase(itr);
				break;
			}
		}
		for (auto itr = baseGameActors.begin(); itr != baseGameActors.end(); itr++)
		{
			if (*itr == arg_object)
			{
				baseGameActors.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::ENERGY_ITEM:
		for (auto itr = energyItems.begin(); itr != energyItems.end(); itr++)
		{
			if (*itr == arg_object)
			{
				energyItems.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::PANEL_ITEM:
		for (auto itr = panelItems.begin(); itr != panelItems.end(); itr++)
		{
			if (*itr == arg_object)
			{
				panelItems.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::UNABLETHROUGH_BLOCK:
		for (auto itr = unableThroughBlocks.begin(); itr != unableThroughBlocks.end(); itr++)
		{
			if (*itr == arg_object)
			{
				unableThroughBlocks.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::UNABLETHROUGH_EDGE:
		for (auto itr = unableThroughEdges.begin(); itr != unableThroughEdges.end(); itr++)
		{
			if (*itr == arg_object)
			{
				unableThroughEdges.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::CIRCULAR_SAW:
		for (auto itr = circularSaws.begin(); itr != circularSaws.end(); itr++)
		{
			if (*itr == arg_object)
			{
				circularSaws.erase(itr);
				break;
			}
		}
		break;
	case ObjectRegistType::FEVERIN_ITEM:
		for (auto itr = feverInItems.begin(); itr != feverInItems.end(); itr++)
		{
			if (*itr == arg_object)
			{
				feverInItems.erase(itr);
				break;
			}
		}
		break;
	default:
		break;
	}
}

void ActorManager::CollisionCheck()
{
	//Actor同士
	for (int i = 0; i < baseGameActors.size() - 1; i++)
	{
		for (int j = i + 1; j < baseGameActors.size(); j++)
		{
			if (baseGameActors[i]->GetActionState()->GetLabel() == ActionStateLabel::FALL)
			{
				break;
			}
			if (baseGameActors[j]->GetActionState()->GetLabel() == ActionStateLabel::FALL)
			{
				continue;
			}

			baseGameActors[i]->HitCheckActor(baseGameActors[j]);
		}
	}

	//アイテム
	for (int i = 0; i < baseGameActors.size(); i++)
	{
		if (baseGameActors[i]->GetActionState()->GetLabel() == ActionStateLabel::FALL)
		{
			continue;
		}

		//まるのこ
		for (int j = 0; j < energyItems.size(); j++)
		{
			baseGameActors[i]->HitCheckEnergyItem(energyItems[j]);
		}

		//パネル
		for (int j = 0; j < panelItems.size(); j++)
		{
			baseGameActors[i]->HitCheckPanelItem(panelItems[j]);
		}

		//スマッシュボール的な
		for (int j = 0; j < feverInItems.size(); j++)
		{
			baseGameActors[i]->HitCheckFeverInItem(feverInItems[j]);
		}
	}
}

Player* ActorManager::GetPlayer()
{
	if (players.empty())
	{
		return nullptr;
	}

	return players[0];
}

std::vector<Field*>& ActorManager::GetFields()
{
	return fields;
}

std::vector<StandardEnemy*>& ActorManager::GetStandardEnemies()
{
	return standardEnemies;
}

std::vector<EnergyItem*>& ActorManager::GetEnergyItems()
{
	return energyItems;
}

std::vector<PanelItem*>& ActorManager::GetPanelItems()
{
	return panelItems;
}

std::vector<UnableThroughBlock*>& ActorManager::GetUnableThroughBlocks()
{
	return unableThroughBlocks;
}

std::vector<UnableThroughEdge*>& ActorManager::GetUnableThroughEdges()
{
	return unableThroughEdges;
}

CircularSaw* ActorManager::GetCircularSaw(Object* arg_obj)
{
	if (circularSaws.empty())
	{
		return nullptr;
	}

	for (auto cs : circularSaws)
	{
		if (cs->GetParentObject() == arg_obj)
		{
			return cs;
		}
	}

	return nullptr;
}

std::vector<BaseGameActor*>& ActorManager::GetBaseGameActors()
{
	return baseGameActors;
}

std::vector<FeverInItem*>& ActorManager::GetFeverInItems()
{
	return feverInItems;
}
