#include "ScoreUI.h"
#include "ScoreManager.h"
#include "Vector.h"

ScoreUI::ScoreUI()
{
	numsp = new NumberSprite(drawScore);
}

ScoreUI::~ScoreUI()
{
	delete numsp;
}

void ScoreUI::Initialize()
{
	drawScore = 0;
}

void ScoreUI::Update()
{
	const int totalScore = ScoreManager::GetInstance()->GetTotalScore();

	//桁に合わせて加算
	if (drawScore < totalScore)
	{
		const int sub = totalScore - drawScore;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else if (sub <= 1000)
		{
			addNum = 100;
		}
		else if (sub <= 10000)
		{
			addNum = 1000;
		}
		else if (sub <= 100000)
		{
			addNum = 10000;
		}
		else
		{
			addNum = 100000;
		}

		drawScore += addNum;
	}
}

void ScoreUI::Draw()
{
	//画像サイズ（数字1つ分）
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//余白
	const Vector2 spaceSize = { 50.0f, 30.0f };

	const Vector2 pos = { (numberTexSize.x / 2) * 6 + spaceSize.x,(numberTexSize.y / 2) + spaceSize.y };
	numsp->Draw(6,"GamePlay_UI_Number", pos);
}
