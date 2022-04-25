#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "TitleText.h"
class DebugCamera;
class Sprite;
class TextureResource;

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
	const std::array<std::array<float,3>,9> lightPos =
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
	
	std::unique_ptr<DebugCamera> camera;
	std::unique_ptr <LightGroup> lightGroup;
	std::vector<std::unique_ptr<Object>> lights;
	std::unique_ptr<Sprite>backTex ;
	std::unique_ptr <TextureResource> resource ;
	std::unique_ptr <TitleText> titleText ;
};
