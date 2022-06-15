#pragma once
#include "Timer.h"
#include <string>
#include <unordered_map>

class InGameTimer
{
public:
	static InGameTimer* GetInstance();
	static void DeleteInstance();

	void Initialize(const int arg_limitSec = 180);
	void Update();

	bool AddTimer(Timer* arg_timer, const std::string& arg_registName);
	Timer* GetTimer(const std::string& arg_timerName = "main");
	std::unordered_map<std::string, Timer*>& GetSubTimers();

private:
	static InGameTimer* instance;
	InGameTimer();
	~InGameTimer();
	InGameTimer(const InGameTimer& another) = delete;
	InGameTimer& operator=(const InGameTimer& another) = delete;

	Timer* mainTimer;
	std::unordered_map<std::string, Timer*> subTimers;
};