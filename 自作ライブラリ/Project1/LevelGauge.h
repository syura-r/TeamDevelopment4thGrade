#pragma once
#include "Sprite.h"

class LevelGauge
{
public:
	LevelGauge();
	~LevelGauge();

	void Initialize();
	void Update();
	void Draw();

private:
	//•‚¢”wŒi
	Sprite* sp_back = nullptr;
	//˜g
	Sprite* sp_frame = nullptr;
	//ƒQ[ƒW‚Ìü
	Sprite* sp_gauge = nullptr;
	int level = 0;
	int panelNum = 0;
	//ƒŒƒxƒ‹•¶š
	Sprite* sp_levelText = nullptr;

};

