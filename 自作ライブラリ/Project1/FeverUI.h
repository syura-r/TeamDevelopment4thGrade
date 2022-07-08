#pragma once
#include "NumberSprite.h"

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
};

