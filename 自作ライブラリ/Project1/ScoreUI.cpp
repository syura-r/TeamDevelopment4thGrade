#include "ScoreUI.h"
#include "ScoreManager.h"
#include "Vector.h"

ScoreUI::ScoreUI()
{
	sp = new Sprite();
	numsp = new NumberSprite(drawScore);
}

ScoreUI::~ScoreUI()
{
	delete sp;
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
	const Vector2 iconTexSize = { 225.0f, 86.0f };
	//画像サイズ（数字1つ分）
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//余白
	const Vector2 spaceSize = { 25.0f, 30.0f };

	//色
	const Vector4 color = { 1,1,1,0.2f };

	Vector2 pos = { (iconTexSize.x / 2) + spaceSize.x,(iconTexSize.y / 2) + spaceSize.y };
	sp->DrawSprite("GamePlay_UI_Score_Text", pos, 0.0f, { 1,1 }, color);

	pos.x += (numberTexSize.x / 2) * 6 + (iconTexSize.x / 2);
	numsp->Draw(6, "GamePlay_UI_Number", pos, { 1,1 }, color);
}
