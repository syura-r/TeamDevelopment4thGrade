#pragma once
#include <unordered_map>
#include <string>

#include "Object.h"
#include "TestBoss.h"

class ActorManager
{
public:
	static ActorManager* GetInstance();
	~ActorManager();

	void Initialize();

	void AddObject(std::string arg_name, Object* arg_object);

	Object* GetGameObject(std::string arg_name);

	TestBoss* GetBoss();

private:
	std::unordered_map<std::string, Object*> mapGameObject;
};

