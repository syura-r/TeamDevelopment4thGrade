#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "Vector.h"

class TimeLimit
{
public:
	TimeLimit(const unsigned int arg_limit);//引数の単位はフレーム
	~TimeLimit();

	void Initialize();
	void Update();
	void Draw();

private:
	void RectChange();

	int limit;

	Timer* timer;

	//分表示
	Sprite* minute_sprite;
	//コロン
	Sprite* colon_sprite;

	//秒の桁数
	static const int secondsDigits = 2;
	//秒表示
	Sprite* seconds_sprite[secondsDigits];

	const std::string numberTexName = "GamePlay_UI_Number";
	//数字1つ分の大きさ
	const Vector2 numberTexSize = { 47.0f,86.0f };
};

