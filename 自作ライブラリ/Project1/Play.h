#pragma once
#include "Scene.h"
#include"LightGroup.h"
#include"CollisionManager.h"
#include "ComputeShade.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include"Grass.h"
#include "Menu.h"
#include "NumberSprite.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Result.h"
#include "Sprite.h"

#include "ActorManager.h"
#include "Pause.h"
#include "TimeLimit.h"
#include "FeverUI.h"
#include "LevelGauge.h"


class TouchAbleObject;
class Stadium;
class ScreenCamera;
class TextureResource;
class Play :public Scene
{
public:
	Play();
	~Play()override;
	void Initialize()override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	void TimeUpdate();
	void KillCountToEnding();
	std::unique_ptr<LightGroup> lightGroup;
	XMFLOAT3 pos = { 1,0,0 };
	float coloramb[3] = { 1,1,1 };
	float color0[3] = { 1,1,1 };
	float color1[3] = { 1,1,1 };
	float color2[3] = { 1,1,1 };
	float lightDir[3] = { 0.0f,-1.00f,0.2f };
	bool targetSphere = false;
	CollisionManager* collisionManager = nullptr;
	ObjectManager* objectManager = nullptr;	
	//std::unique_ptr<Menu> menu;

	Stadium* stadium = nullptr;

	TextureResource* screenResource = nullptr;

	ActorManager* actorManager = nullptr;
	
	ScreenCamera* screenCamera = nullptr;


	//std::unique_ptr<DebugCamera> camera;
	std::unique_ptr<InGameCamera> camera;
	std::array<std::array<float,3>,9>lightPos = 
	{
		-6.000f,19.404f,2.137f,
		-6.000f,19.404f,30.627f,
		-6.000f,19.404f,-25.783f,
		18.000f,19.404f,2.137f,
		18.000f,19.404f,30.627f,
		18.000f,19.404f,-25.783f,
	   -26.571f,19.404f,2.137f,
	   -26.571f,19.404f,30.627f,
	   -26.571f,19.404f,-25.783f,
	};
	
	std::array<float, 3> lightAtten = { 1,1,1 };

	Pause* pause = nullptr;
	TimeLimit* timeLimit = nullptr;
	int gameEndCount;
	FeverUI* feverUI = nullptr;
	LevelGauge* levelGauge = nullptr;
};

