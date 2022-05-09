#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "TitleText.h"
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
};
