#include "TimeLimit.h"

TimeLimit::TimeLimit(const unsigned int arg_limit)
{
	limit = arg_limit;
	timer = new Timer(arg_limit);

	minute_sprite = new Sprite();
	colon_sprite = new Sprite();
	for (int i = 0; i < secondsDigits; i++)
	{
		seconds_sprite[i] = new Sprite();
	}
}

TimeLimit::~TimeLimit()
{
	delete timer;
	delete minute_sprite;
	delete colon_sprite;
	for (int i = 0; i < secondsDigits; i++)
	{
		delete seconds_sprite[i];
	}
}

void TimeLimit::Initialize()
{
	timer->SetLimit(limit, true);
}

void TimeLimit::Update()
{
	timer->Update();

	RectChange();
}

void TimeLimit::Draw()
{
	//分の左上座標
	Vector2 position = { 1920 - ((numberTexSize.x * 3.5f) + 30),10 };
	const Vector2 scale = { 1,1 };
	const Vector4 color = { 1,1,1,1 };

	minute_sprite->DrawSprite(numberTexName, position, 0.0f, scale, color, { 0,0 });

	//１桁分ずらす
	position.x += numberTexSize.x;
	colon_sprite->DrawSprite("GamePlay_UI_Colon", position, 0.0f, scale, color, { 0,0 });

	//コロンの大きさずらす
	position.x += numberTexSize.x / 2;
	for (int i = 0; i < secondsDigits; i++)
	{
		seconds_sprite[i]->DrawSprite(numberTexName, position, 0.0f, scale, color, { 0,0 });
		//１桁分ずらす
		position.x += numberTexSize.x;
	}
}

void TimeLimit::RectChange()
{
	//残り時間（秒）
	const int nowTime = timer->GetRealTime(TimerPerformance::Down);
	//1分は60秒
	const int secondsToMinute = 60;
	//表示時間（分）
	const int minuteTime = nowTime / secondsToMinute;
	//表示時間（秒）
	const int secondsTime = nowTime % secondsToMinute;

	//桁ごとの数（秒）
	const int secondsDigitsNum[secondsDigits] = { secondsTime / 10, secondsTime % 10 };

	//画像切り抜き
	minute_sprite->SpriteSetTextureRect(numberTexName,
		numberTexSize.x * minuteTime, 0,
		numberTexSize.x, numberTexSize.y);
	for (int i = 0; i < secondsDigits; i++)
	{
		seconds_sprite[i]->SpriteSetTextureRect(numberTexName,
			numberTexSize.x * secondsDigitsNum[i], 0,
			numberTexSize.x, numberTexSize.y);
	}
}
