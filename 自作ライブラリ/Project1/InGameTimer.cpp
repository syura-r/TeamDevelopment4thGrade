#include "InGameTimer.h"

InGameTimer* InGameTimer::instance = nullptr;

InGameTimer::InGameTimer()
	:mainTimer(new Timer(60))
{
	subTimers.clear();
}

InGameTimer::~InGameTimer()
{
	delete mainTimer;
	for (auto t : subTimers)
	{
		delete t.second;
		t.second = nullptr;
	}
	subTimers.clear();
}

InGameTimer* InGameTimer::GetInstance()
{
	if (!instance)
	{
		instance = new InGameTimer();
	}
	return instance;
}

void InGameTimer::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void InGameTimer::Initialize(const int arg_limitSec)
{
	mainTimer->SetLimit(arg_limitSec * 60, true);
	for (auto t : subTimers)
	{
		t.second->Reset();
	}	
}

void InGameTimer::Update()
{
	mainTimer->Update();
	for (auto t : subTimers)
	{
		t.second->Update();
	}
}

bool InGameTimer::AddTimer(Timer* arg_timer, const std::string& arg_registName)
{
	if (arg_registName == "main")
	{
		return false;
	}

	if (subTimers.find(arg_registName) != subTimers.end())
	{
		return false;
	}

	subTimers.emplace(arg_registName, arg_timer);
	return true;
}

Timer* InGameTimer::GetTimer(const std::string& arg_timerName)
{
	if (arg_timerName == "main")
	{
		return mainTimer;
	}

	auto itr = subTimers.find(arg_timerName);
	if (itr != subTimers.end())
	{
		return itr->second;
	}
	return nullptr;
}

std::unordered_map<std::string, Timer*>& InGameTimer::GetSubTimers()
{
	return subTimers;
}
