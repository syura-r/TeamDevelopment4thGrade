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
	//�ꖇ������̑f�_
	const int rawScore = 1000;
	//�����؂蔲���{��
	const float onceBonus = 1.0f + ((float)oncePanelNum / 10.0f);

	result = rawScore * onceBonus * oncePanelNum;

	totalScore += result;
}

ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}
