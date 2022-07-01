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
	color = { 1,1,1,0.8f };
}

void TimeLimit::Update()
{
	//�c�莞�ԁi�b�j
	const int nowTime = timer->GetRealTime(TimerPerformance::Down);
	//�\�����ԁi���j
	minute = nowTime / 60;
	//�\�����ԁi�b�j
	seconds = nowTime % 60;

	timer->Update();

	//�I���ԍۂɐF��ς���
	const int redTime = 10;
	if (nowTime <= redTime)
	{
		color.x = 1.0f;
		color.y = 0.0f;
		color.z = 0.0f;
	}
}

void TimeLimit::Draw()
{
	//�摜�T�C�Y�i����1���j
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//��ʃT�C�Y
	const Vector2 windowSize = { 1920.0f, 1080.0f };

	//���W
	Vector2 position = { windowSize.x / 2, windowSize.y / 4};

	position.x -= numberTexSize.x;//���̒��S�ɂ��炷
	position.y -= numberTexSize.y / 2;
	minute_sprite->Draw(1, "GamePlay_UI_Number", position, { 1,1 }, color, { 0.5f,0.5f },"NoAlphaToCoverageSprite");

	position.x += (numberTexSize.x / 4) * 3;//�R�����̒��S�ɂ��炷
	colon_sprite->DrawSprite("GamePlay_UI_Colon", position, 0, { 1,1 }, color, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");

	position.x += (numberTexSize.x / 4) * 5;//�b�̒��S�ɂ��炷
	seconds_sprite->Draw(2, "GamePlay_UI_Number", position, { 1,1 }, color, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}
