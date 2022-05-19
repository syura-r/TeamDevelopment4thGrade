#include "ActorManager.h"

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
