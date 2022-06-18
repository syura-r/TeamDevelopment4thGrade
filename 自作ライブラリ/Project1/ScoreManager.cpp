#include "ScoreManager.h"

ScoreManager* ScoreManager::sManager = nullptr;

ScoreManager* ScoreManager::GetInstance()
{
	if (!sManager)
	{
		sManager = new ScoreManager();
	}
	return sManager;
}

void ScoreManager::Finalize()
{
	delete sManager;
}

void ScoreManager::Inisitlize()
{
	totalScore = 0;
	stockPanelNum_last = 0;
}

void ScoreManager::AddScore(const int oncePanelNum)
{
	int result = 0;
	//ˆê–‡‚ ‚½‚è‚Ì‘f“_
	const int rawScore = 1000;
	//“¯Ø‚è”²‚«”{—¦
	const float onceBonus = 1 + (oncePanelNum / 10);

	result = rawScore * onceBonus * oncePanelNum;

	totalScore += result;
}

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}
