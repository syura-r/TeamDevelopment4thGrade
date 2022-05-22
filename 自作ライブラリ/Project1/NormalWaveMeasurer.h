#pragma once
#include "Sprite.h"
#include "NumberSprite.h"
#include "Timer.h"

class NormalWaveMeasurer
{
public:
	NormalWaveMeasurer();
	~NormalWaveMeasurer();

	void Initialize();
	void Update();
	void Draw();

	bool IsTime();
	void Reset();
	void Reset(const float arg_sec);

private:
	Timer* timer;
	Sprite* nowTimeSprite;
	Sprite* baseSprite;
	NumberSprite* nowSecSprite;
	float nowSec;
};