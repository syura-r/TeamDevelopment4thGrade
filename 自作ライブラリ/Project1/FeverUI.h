#pragma once
#include "NumberSprite.h"
#include "Vector.h"

class FeverUI
{
public:
	FeverUI();
	~FeverUI();

	void Initialize();
	void Update();
	void Draw();

private:
	float drawNum;

	Sprite* sp;
	NumberSprite* numsp;

	std::string texName;

	bool isInFever;
	Vector2 position;
	Vector2 scale;
	Vector4 color;
};

