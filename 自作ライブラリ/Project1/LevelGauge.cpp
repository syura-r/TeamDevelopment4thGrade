#include "LevelGauge.h"
#include "ActorManager.h"
#include "Player.h"

LevelGauge::LevelGauge()
{
	sp_back = new Sprite();
	for (int i = 0; i < nextLevelCount; i++)
	{
		sp_tri[i] = new Sprite();
	}
	sp_levelText = new Sprite();
}

LevelGauge::~LevelGauge()
{
	delete sp_back;
	for (int i = 0; i < nextLevelCount; i++)
	{
		delete sp_tri[i];
	}
	delete sp_levelText;
}

void LevelGauge::Initialize()
{
	color_tri_all = colorBlank;

	level = 1;
	gaugeCount = 0;

	newlevel = false;
	prev_level = level;
	timer_newLevelAction = 0;
}

void LevelGauge::Update()
{
	prev_level = level;

	Player* player = ActorManager::GetInstance()->GetPlayer();
	//ボーナスレベル
	const int maxLevel = 3;
	level = player->GetBonusCount() / nextLevelCount + 1;
	//ボーナスパネル切り抜き量
	gaugeCount = player->GetBonusCount() - (nextLevelCount * (level - 1));


	if (level >= maxLevel)
	{
		level = maxLevel;
		gaugeCount = nextLevelCount;
	}

	if (prev_level < level)
	{
		newlevel = true;
	}
	if (newlevel)
	{
		NewLevelAction();
	}
}

void LevelGauge::Draw()
{
	const Vector2 texSize = { 180,560 };
	const Vector2 position_space = { 40, 30 };
	const Vector2 position = { texSize.x / 2 + position_space.x, 1080.0f - (texSize.y / 2) - position_space.y };

	sp_levelText->DrawSprite("GamePlay_UI_Level" + std::to_string(level), position);

	for (int i = 0; i < nextLevelCount; i++)
	{
		Vector4 color = colorBlank;
		if (i < gaugeCount)
		{
			color = colorYellow;
		}
		if(newlevel)
		{
			color = color_tri_all;
		}
		sp_tri[i]->DrawSprite("GamePlay_UI_Leveltri_" + std::to_string(i + 1), position, 0.0f, { 1.0f,1.0f }, color);
	}

	sp_back->DrawSprite("GamePlay_UI_Levelback", position);
}

void LevelGauge::NewLevelAction()
{
	const int timerLimit = 60;
	timer_newLevelAction++;

	//ゲージ最大で保持
	gaugeCount = nextLevelCount;

	//点滅
	const int interval = 10;
	int stockTimer = timer_newLevelAction;
	int intervalCount = 0;
	while (stockTimer >= interval)
	{
		stockTimer -= interval;
		intervalCount++;
	}
	if (intervalCount % 2)
	{
		color_tri_all = colorBlank;
	}
	else
	{
		color_tri_all = colorYellow;
	}

	//終了
	if (timer_newLevelAction >= timerLimit)
	{
		timer_newLevelAction = 0;
		color_tri_all = colorBlank;
		newlevel = false;
	}
}
