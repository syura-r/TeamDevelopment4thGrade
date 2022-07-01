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
	fallEnemyNum = 0;
}

void ScoreManager::AddScore_CutPanel(const int oncePanelNum)
{
	int result = 0;
	//ˆê–‡‚ ‚½‚è‚Ì‘f“_
	int rawScore = 1000;
	switch (oncePanelNum)
	{
	case 1:
		rawScore = 1000;
		break;
	case 2:
		rawScore = 1250;
		break;
	case 3:
		rawScore = 1500;
		break;
	case 4:
		rawScore = 2000;
		break;
	case 5:
		rawScore = 2500;
		break;
	case 6:
		rawScore = 3000;
		break;
	default:
		break;
	}
	result = rawScore * oncePanelNum;
	totalScore += result;
}

void ScoreManager::AddScore_FallEnemy()
{
	const int result = 5000;
	totalScore += result;

	fallEnemyNum++;
}

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}
