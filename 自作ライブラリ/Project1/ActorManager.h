#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"

class Player;
class TestBoss;

class ActorManager
{
public:
	static ActorManager* GetInstance();
	~ActorManager();

	void Initialize();

	void AddObject(std::string arg_name, Object* arg_object);

	Object* GetGameObject(std::string arg_name);

	Player* GetPlayer();
	TestBoss* GetBoss();

private:
	std::unordered_map<std::string, Object*> mapGameObject;
};

