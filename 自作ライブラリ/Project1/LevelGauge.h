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

	//いくつ毎にレベルが上がるか
	static const int nextLevelCount = 5;

	//三角形ゲージ
	Sprite* sp_tri[nextLevelCount];
	Vector4 color_tri_all;
	const Vector4 colorBlank = { 0.25f, 0.25f, 0.25f, 1 };
	const Vector4 colorYellow = { 0.88f, 0.96f, 0.0f, 1 };

	int level = 0;
	int gaugeCount = 0;

	//レベル文字
	Sprite* sp_levelText = nullptr;

	//レベルが変わった時
	bool newlevel = false;
	int prev_level = 0;
	int timer_newLevelAction = 0;
	void NewLevelAction();
};

