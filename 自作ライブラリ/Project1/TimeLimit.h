#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "Vector.h"

class TimeLimit
{
public:
	TimeLimit(const unsigned int arg_limit);//�����̒P�ʂ̓t���[��
	~TimeLimit();

	void Initialize();
	void Update();
	void Draw();

private:
	void RectChange();

	int limit;

	Timer* timer;

	//���\��
	Sprite* minute_sprite;
	//�R����
	Sprite* colon_sprite;

	//�b�̌���
	static const int secondsDigits = 2;
	//�b�\��
	Sprite* seconds_sprite[secondsDigits];

	const std::string numberTexName = "GamePlay_UI_Number";
	//����1���̑傫��
	const Vector2 numberTexSize = { 47.0f,86.0f };
};

