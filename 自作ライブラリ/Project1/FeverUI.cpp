#include "FeverUI.h"
#include "ActorManager.h"
#include "Field.h"
#include "Player.h"
#include "StandardEnemy.h"

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
	isInFever = false;
	position = {};
	scale = { 1,1 };
	color = { 1,1,1,1 };
}

void FeverUI::Update()
{
	//
	ActorManager* aManager = ActorManager::GetInstance();

	//画像サイズ
	const Vector2 textTexSize = { 746.0f, 54.0f };
	//フィーバー中か
	isInFever = aManager->GetPlayer()->IsInFever() ||
		aManager->GetStandardEnemies()[0]->IsInFever() ||
		aManager->GetStandardEnemies()[1]->IsInFever();

	//フィーバー時のいろいろ変更
	if (isInFever)
	{
		texName = "GamePlay_UI_FeverTimeText";
		Vector2 spaceSize = { 0.0f, 60.0f };//余白
		position = { 960 + spaceSize.x, (textTexSize.y / 2) + spaceSize.y };
		scale = { 1.3f,1.3f };
		//色相変化
		const float speed = 0.1f;
		if (color.x >= 1.0f && color.y >= 1.0f && color.z >= 1.0f)
		{
			//白（初期値）なら赤へ
			color = { 1.0f,0.0f,0.0f,1 };
		}
		else if (color.x >= 1.0f && color.z <= 0.0f
			&& color.y < 1.0f)
		{
			//赤なら緑を増やして黄色へ
			color.y += speed;
		}
		else if (color.y >= 1.0f && color.z <= 0.0f
			&& color.x > 0.0f)
		{
			//黄色なら赤を減らして緑へ
			color.x -= speed;
		}
		else if (color.y >= 1.0f && color.x <= 0.0f
			&& color.z < 1.0f)
		{
			//緑なら青を増やして水色へ
			color.z += speed;
		}
		else if (color.z >= 1.0f && color.x <= 0.0f
			&& color.y > 0.0f)
		{
			//水色なら緑を減らして青へ
			color.y -= speed;
		}
		else if (color.z >= 1.0f && color.y <= 0.0f
			&& color.x < 1.0f)
		{
			//青なら赤を増やして紫へ
			color.x += speed;
		}
		else if (color.x >= 1.0f && color.y <= 0.0f
			&& color.z > 0.0f)
		{
			//紫なら青を減らして赤へ
			color.z -= speed;
		}
		return;
	}
	else//フィーバー時以外
	{
		texName = "GamePlay_UI_FeverText";
		Vector2 spaceSize = { 25.0f, 30.0f };//余白
		position = { (textTexSize.x / 2) + spaceSize.x, (textTexSize.y / 2) + spaceSize.y };
		scale = { 1.0f,1.0f };
		color = { 1.0f,1.0f,1.0f,1.0f };
	}

	//数字の変動
	Field* field = aManager->GetFields()[0];
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
	sp->DrawSprite(texName, position, 0.0f, scale, color);

	if (isInFever)
	{
		//フィーバー時は数字の描画はしない
		return;
	}

	const float numberSpaceAddSize = 85.0f;
	position.x += numberSpaceAddSize;//文字画像の数字の配置する位置まで
	std::string nuwNum = std::to_string((int)drawNum);
	int digit = nuwNum.size();//0で埋めないため
	numsp->Draw(digit, "GamePlay_UI_Number", position);
}
