#include "ScoreRanking.h"
#include "ActorManager.h"
#include "Player.h"
#include "StandardEnemy.h"

ScoreRanking::ScoreRanking()
{
	back = new Sprite();
	blue_UI = new ActorScoreUI("GamePlay_Score_Player");
	red_UI = new ActorScoreUI("GamePlay_Score_Enemy");
	green_UI = new ActorScoreUI("GamePlay_Score_Enemy2");

}

ScoreRanking::~ScoreRanking()
{
	delete back;
	delete blue_UI;
	delete red_UI;
	delete green_UI;
}

void ScoreRanking::Initialize()
{
	const XMFLOAT2 backSize = {504,240};
	position_base = { 1920 - backSize.x, 1080 - backSize.y };

	blue_UI->Initialize();
	red_UI->Initialize();
	green_UI->Initialize();
}

void ScoreRanking::Update()
{

	CheckScore();
	CheckRank();
	CheckPosition();

	blue_UI->Update();
	red_UI->Update();
	green_UI->Update();
}

void ScoreRanking::Draw()
{
	blue_UI->Draw(position_base.x);
	red_UI->Draw(position_base.x);
	green_UI->Draw(position_base.x);

	back->DrawSprite("GamePlay_Score_Rank", position_base, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.0f, 0.0f });
}

void ScoreRanking::CheckScore()
{
	ActorManager* actorManager = ActorManager::GetInstance();
	blue_UI->score = actorManager->GetPlayer()->GetTotalCutCount();
	red_UI->score = actorManager->GetStandardEnemies()[0]->GetTotalCutCount();
	green_UI->score = actorManager->GetStandardEnemies()[1]->GetTotalCutCount();
}

void ScoreRanking::CheckRank()
{
	//blue_UI->rank = 1;
	//red_UI->rank = 2;
	//green_UI->rank = 1;
}

void ScoreRanking::CheckPosition()
{
	//ˆÊ’u
	const float space = 3;
	const float iconSize_y = 74;
	float position_y[3] = {};
	for (int i = 0; i < 3; i++)
	{
		position_y[i] = position_base.y +
			(iconSize_y * i) +
			(space * (i + 2));
	}

	blue_UI->position.y = position_y[0];
	red_UI->position.y = position_y[1];
	green_UI->position.y = position_y[2];

	/*
	//“¯‡ˆÊ‚È‚ç‹ó‚¢‚Ä‚¢‚é‚Æ‚±‚ë‚Ü‚ÅˆÚ“®
	bool isSet[3] = { false, false, false };
	int add = 0;

	//‡ˆÊ‚Ì‚‚¢‡‚É


	add = 0;
	while (isSet[(int)blue_UI->rank + add - 1])
	{
		add++;
	}
	blue_UI->position.y = position_y[(int)blue_UI->rank + add - 1];
	isSet[(int)blue_UI->rank + add - 1] = true;

	add = 0;
	while (isSet[(int)red_UI->rank + add - 1])
	{
		add++;
	}
	red_UI->position.y = position_y[(int)red_UI->rank + add - 1];
	isSet[(int)red_UI->rank + add - 1] = true;


	add = 0;
	while (isSet[(int)green_UI->rank + add - 1])
	{
		add++;
	}
	green_UI->position.y = position_y[(int)green_UI->rank + add - 1];
	isSet[(int)green_UI->rank + add - 1] = true;
	*/
}


ScoreRanking::ActorScoreUI::ActorScoreUI(const std::string& arg_texName)
{
	icon = new Sprite();
	cross = new Sprite();
	score_sp = new NumberSprite(score);
	rank_sp = new NumberSprite(rank);
	texName = arg_texName;
}

ScoreRanking::ActorScoreUI::~ActorScoreUI()
{
	delete icon;
	delete cross;
	delete score_sp;
	delete rank_sp;
}

void ScoreRanking::ActorScoreUI::Initialize()
{
	score = 0;
	position = {};
	rank = 1;
}

void ScoreRanking::ActorScoreUI::Update()
{
}

void ScoreRanking::ActorScoreUI::Draw(const float arg_basePosition_x)
{
	position.x = arg_basePosition_x + 90.0f;
	int digit = 1;
	rank_sp->Draw(digit, "GamePlay_UI_Number", position, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.0f,0.0f });

	position.x += 80.0f;
	icon->DrawSprite(texName, position, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, {0.0f,0.0f});

	const float iconSize_x = 100.0f;
	position.x += iconSize_x;
	cross->DrawSprite("GamePlay_Score_Batu", position, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.0f,0.0f });

	const float crossSize_x = 100.0f;
	const float space = 10.0f;
	position.x += crossSize_x + space;
	digit = 2;
	score_sp->Draw(digit, "GamePlay_UI_Number", position, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.0f,0.0f });
}
