#pragma once
#include "Timer.h"
#include "NumberSprite.h"

class TimeLimit
{
public:
	TimeLimit(const unsigned int arg_limit);//引数の単位はフレーム
	~TimeLimit();

	void Initialize();
	void Update();
	void Draw();

	bool GetLimit() { timer->IsTime(); }

private:
	int limit;
	float minute;
	float seconds;

	Timer* timer;

	//分表示
	NumberSprite* minute_sprite;
	//コロン
	Sprite* colon_sprite;
	//秒表示
	NumberSprite* seconds_sprite;
};

