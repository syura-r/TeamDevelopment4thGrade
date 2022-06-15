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
	//���̍�����W
	Vector2 position = { 1920 - ((numberTexSize.x * 3.5f) + 30),10 };
	const Vector2 scale = { 1,1 };
	const Vector4 color = { 1,1,1,1 };

	minute_sprite->DrawSprite(numberTexName, position, 0.0f, scale, color, { 0,0 });

	//�P�������炷
	position.x += numberTexSize.x;
	colon_sprite->DrawSprite("GamePlay_UI_Colon", position, 0.0f, scale, color, { 0,0 });

	//�R�����̑傫�����炷
	position.x += numberTexSize.x / 2;
	for (int i = 0; i < secondsDigits; i++)
	{
		seconds_sprite[i]->DrawSprite(numberTexName, position, 0.0f, scale, color, { 0,0 });
		//�P�������炷
		position.x += numberTexSize.x;
	}
}

void TimeLimit::RectChange()
{
	//�c�莞�ԁi�b�j
	const int nowTime = timer->GetRealTime(TimerPerformance::Down);
	//1����60�b
	const int secondsToMinute = 60;
	//�\�����ԁi���j
	const int minuteTime = nowTime / secondsToMinute;
	//�\�����ԁi�b�j
	const int secondsTime = nowTime % secondsToMinute;

	//�����Ƃ̐��i�b�j
	const int secondsDigitsNum[secondsDigits] = { secondsTime / 10, secondsTime % 10 };

	//�摜�؂蔲��
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
