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

void ScoreManager::AddScore_CutPanel(const int oncePanelNum)
{
	//int result = 0;
	////ˆê–‡‚ ‚½‚è‚Ì‘f“_
	//const int rawScore = 1000;
	////“¯Ø‚è”²‚«”{—¦
	//const float onceBonus = 1.0f + ((float)oncePanelNum / 10.0f);

	//result = rawScore * onceBonus * oncePanelNum;

	//totalScore += result;


	int result = 0;
	switch (oncePanelNum)
	{
	case 1:
		result = 1000;
		break;
	case 2:
		result = 1250;
		break;
	case 3:
		result = 1500;
		break;
	case 4:
		result = 2000;
		break;
	case 5:
		result = 2500;
		break;
	case 6:
		result = 3000;
		break;
	default:
		break;
	}

	totalScore += result;
}

void ScoreManager::AddScore_FallEnemy()
{
	const int result = 5000;
	totalScore += result;
}

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}
