#pragma once
#include "Sprite.h"
#include "Timer.h"

class NormalWaveMeasurer
{
public:
	NormalWaveMeasurer();
	~NormalWaveMeasurer();

	void Initialize();
	void Update();
	void Draw();

private:
	Timer* timer;
	Sprite* nowTimeSprite;
	Sprite* baseSprite;
};