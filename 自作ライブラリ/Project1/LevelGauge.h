#pragma once
#include "Sprite.h"
#include "Vector.h"

class LevelGauge
{
public:
	LevelGauge();
	~LevelGauge();

	void Initialize();
	void Update();
	void Draw();

private:
	//黒い背景
	Sprite* sp_back = nullptr;
	//枠
	Sprite* sp_frame = nullptr;
	//ゲージの線
	Sprite* sp_gauge = nullptr;
	Vector4 colorGauge = { 1,1,1,1 };

	int level = 0;
	int nextLevelCount = 0;//いくつ毎にレベルが上がるか
	const int maxGaugeStep = 15;//ゲージの総段数
	int gaugeCount = 0;

	//レベル文字
	Sprite* sp_levelText = nullptr;

	//レベルが変わった時
	bool newlevel = false;
	int prev_level = 0;
	int timer_newLevelAction = 0;
	void NewLevelAction();
};

