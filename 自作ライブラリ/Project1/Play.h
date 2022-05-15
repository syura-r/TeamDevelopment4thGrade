#pragma once
#include "Scene.h"
#include"LightGroup.h"
#include"CollisionManager.h"
#include "ComputeShade.h"
#include "DebugCamera.h"
#include"Grass.h"
#include "Menu.h"
#include "NumberSprite.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Result.h"
#include "Sprite.h"

#include "Line.h"
#include "TestStar.h"

class TouchAbleObject;
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
	std::unique_ptr<LightGroup> lightGroup;
	XMFLOAT3 pos = { 1,0,0 };
	float coloramb[3] = { 1,1,1 };
	float color0[3] = { 1,1,1 };
	float color1[3] = { 1,1,1 };
	float color2[3] = { 1,1,1 };
	float lightDir[3] = {1.0f,-0.6f,0.0f};
	bool targetSphere = false;
	CollisionManager* collisionManager = nullptr;
	ObjectManager* objectManager = nullptr;
	Player* player =nullptr;
	std::unique_ptr<Menu> menu;

	Line* line = nullptr;
	TestStar* testStar = nullptr;
		
	std::unique_ptr<Result> result;
	std::unique_ptr<DebugCamera> camera;
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
};

