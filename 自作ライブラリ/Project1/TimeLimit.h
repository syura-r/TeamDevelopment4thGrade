#pragma once
#include "Timer.h"
#include "NumberSprite.h"

class TimeLimit
{
public:
	TimeLimit(const unsigned int arg_limit);//�����̒P�ʂ̓t���[��
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

	//���\��
	NumberSprite* minute_sprite;
	//�R����
	Sprite* colon_sprite;
	//�b�\��
	NumberSprite* seconds_sprite;
};

