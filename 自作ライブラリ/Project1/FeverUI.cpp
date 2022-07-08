#include "FeverUI.h"
#include "Vector.h"
#include "ActorManager.h"
#include "Field.h"

FeverUI::FeverUI()
{
	sp = new Sprite();
	numsp = new NumberSprite(drawNum);
}

FeverUI::~FeverUI()
{
	delete sp;
	delete numsp;
}

void FeverUI::Initialize()
{
	drawNum = 0;
}

void FeverUI::Update()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	int cutPanelNum = field->GetGottenCount();
	while (cutPanelNum >= field->GetFeverNolma())
	{
		cutPanelNum -= field->GetFeverNolma();
	}
	const int toFeverNum = field->GetFeverNolma() - cutPanelNum;

	//桁に合わせて加算
	if (drawNum < toFeverNum)
	{
		const int sub = toFeverNum - drawNum;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else
		{
			addNum = 100;
		}

		drawNum += addNum;
	}
	//桁に合わせて減算
	else if (drawNum > toFeverNum)
	{
		const int sub = drawNum - toFeverNum;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else
		{
			addNum = 100;
		}

		drawNum -= addNum;
	}
}

void FeverUI::Draw()
{
	//画像サイズ
	const Vector2 textTexSize = { 746.0f, 54.0f };
	//余白
	const Vector2 spaceSize = { 25.0f, 30.0f };

	Vector2 pos = { (textTexSize.x / 2) + spaceSize.x,(textTexSize.y / 2) + spaceSize.y };
	sp->DrawSprite("GamePlay_UI_FeverText", pos);

	const float numberSpaceAddSize = 85.0f;
	pos.x += numberSpaceAddSize;//文字画像の数字の配置する位置まで
	std::string nuwNum = std::to_string((int)drawNum);
	int digit = nuwNum.size();//0で埋めないため
	numsp->Draw(digit, "GamePlay_UI_Number", pos);
}
