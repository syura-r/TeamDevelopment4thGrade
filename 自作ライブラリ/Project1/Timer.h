#pragma once
#include "FPS.h"

enum class TimerPerformance
{
	Up,
	Down,
};

class Timer
{
public:
	Timer();
	Timer(const unsigned int arg_limit);
	~Timer();

	void Initialize();
	void Update();
	
	void Reset();	
	int GetLimit()const;
	void SetLimit(const unsigned int arg_limit, const bool arg_isReset = false);
	//�o�ߎ��ԁi�t���[���j
	int GetTime(const TimerPerformance arg_performance = TimerPerformance::Up)const;
	//�o�ߎ��ԁi�b�j
	float GetRealTime(const TimerPerformance arg_performance = TimerPerformance::Up)const;
	//�o�ߗ�
	float GetRate(const TimerPerformance arg_performance = TimerPerformance::Up)const;
	
	bool IsTime()const;
	
	static void SetFPS(const float arg_fps);

private:
	unsigned int nowCount;
	unsigned int limitCount;
	static float fps;
};