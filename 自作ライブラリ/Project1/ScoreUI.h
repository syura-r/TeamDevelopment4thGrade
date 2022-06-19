#pragma once
#include "NumberSprite.h"

class ScoreUI
{
public:
	ScoreUI();
	~ScoreUI();

	void Initialize();
	void Update();
	void Draw();

private:
	float drawScore;

	NumberSprite* numsp;
};

