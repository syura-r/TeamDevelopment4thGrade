#include "TimeLimit.h"
#include "Vector.h"

TimeLimit::TimeLimit(const unsigned int arg_limit)
{
	limit = arg_limit;
	minute = limit / 60;
	seconds = limit % 60;
	timer = new Timer(arg_limit);

	minute_sprite = new NumberSprite(minute);
	colon_sprite = new Sprite();
	seconds_sprite = new NumberSprite(seconds);
}

TimeLimit::~TimeLimit()
{
	delete timer;
	delete minute_sprite;
	delete colon_sprite;
	delete seconds_sprite;
}

void TimeLimit::Initialize()
{
	timer->SetLimit(limit, true);
}

void TimeLimit::Update()
{
	//残り時間（秒）
	const int nowTime = timer->GetRealTime(TimerPerformance::Down);
	//表示時間（分）
	minute = nowTime / 60;
	//表示時間（秒）
	seconds = nowTime % 60;

	timer->Update();
}

void TimeLimit::Draw()
{
	//画像サイズ（数字1つ分）
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//画面サイズ
	const Vector2 windowSize = { 1920.0f, 1080.0f };
	//余白
	const Vector2 spaceSize = { 50.0f, 30.0f };

	//全体の左上座標
	Vector2 position = { windowSize.x - (numberTexSize.x * 3) - spaceSize.x, (numberTexSize.y / 2) + spaceSize.y };

	minute_sprite->Draw(1, "GamePlay_UI_Number", position);

	position.x += (numberTexSize.x / 4) * 3;
	colon_sprite->DrawSprite("GamePlay_UI_Colon", position);

	position.x += (numberTexSize.x / 4) * 5;
	seconds_sprite->Draw(2, "GamePlay_UI_Number", position);
}
