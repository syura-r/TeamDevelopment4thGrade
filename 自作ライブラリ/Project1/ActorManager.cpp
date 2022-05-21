#include "ActorManager.h"
#include "Player.h"
#include "TestBoss.h"

ActorManager* ActorManager::GetInstance()
{
	static ActorManager* instance = new ActorManager();
	return instance;
}



ActorManager::~ActorManager()
{
	mapGameObject.clear();//�o�^���Ă���I�u�W�F�N�g�͕ʂ̏ꏊ��delete�����
}

void ActorManager::Initialize()
{
	mapGameObject.clear(); 
}

void ActorManager::AddObject(std::string arg_name, Object* arg_object)
{
	if (mapGameObject[arg_name] != nullptr)//���ɂ��̖��O�ō쐬���ꂽ�I�u�W�F�N�g������ꍇ�͏I��
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
