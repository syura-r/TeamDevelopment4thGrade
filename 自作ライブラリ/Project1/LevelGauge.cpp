#include "LevelGauge.h"
#include "ActorManager.h"
#include "Player.h"

LevelGauge::LevelGauge()
{
	sp_back = new Sprite();
	sp_frame = new Sprite();
	sp_gauge = new Sprite();
	sp_levelText = new Sprite();
}

LevelGauge::~LevelGauge()
{
	delete sp_back;
	delete sp_frame;
	delete sp_gauge;
	delete sp_levelText;
}

void LevelGauge::Initialize()
{
	colorGauge = { 1,1,1,1 };

	level = 1;
	nextLevelCount = 5;
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

	//colorGauge = { 1,1,1.0f - (0.3f * level),1 };

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
	Vector2 texSize = { 180,540 };
	Vector2 position = { texSize.x / 2,1080.0f - texSize.y + 37.0f };

	sp_levelText->DrawSprite("GamePlay_UI_Level" + std::to_string(level), position);

	position.y = 1080.0f;
	const int step = maxGaugeStep / nextLevelCount * gaugeCount;//何段表示するか
	const float stepSize = 30.0f;//1段の大きさ
	const float spaceSize = 25.0f;//余白の大きさ
	sp_gauge->SpriteSetTextureRect("GamePlay_UI_Levelgauge", 0.0f, texSize.y - (stepSize * step + spaceSize), texSize.x, stepSize * step + spaceSize);
	sp_gauge->DrawSprite("GamePlay_UI_Levelgauge", position, 0.0f, { 1.0f,1.0f }, colorGauge, { 0.5f, 1.0f });
	position.y = 1080.0f - (texSize.y / 2.0f);
	sp_frame->DrawSprite("GamePlay_UI_Leveledge", position);
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
		colorGauge = { 1,1,1,1 };
	}
	else
	{
		colorGauge = { 1,1,0,1 };
	}

	//終了
	if (timer_newLevelAction >= timerLimit)
	{
		timer_newLevelAction = 0;
		colorGauge = { 1,1,1,1 };
		newlevel = false;
	}
}
