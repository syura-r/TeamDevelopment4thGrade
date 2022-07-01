#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "TitleText.h"
#include "Sprite.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include"LightGroup.h"

class Title :
	public Scene
{
public:
	Title();
	~Title()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	std::unique_ptr<DebugCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	Sprite* titleLogo = nullptr;
	Sprite* titleStart = nullptr;

	struct Panel
	{
		Panel();
		~Panel();

		Object3D* object = nullptr;
		Vector3 position = { 0,0,0 };
		Vector3 scale = { 1,1,1 };
		Vector3 rotation = { 0,0,0 };
		Vector4 color = { 0.25f, 0.58f, 1.0f, 1.0f };
	};

	static const int panelsNum_X = 15;
	static const int panelsNum_Y = 9;
	static const int panelsNum_ALL = panelsNum_X * panelsNum_Y * 2;
	Panel* panels[panelsNum_ALL];
};
