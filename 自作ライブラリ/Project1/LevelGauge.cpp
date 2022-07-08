#include "LevelGauge.h"
#include "Vector.h"
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
	level = 1;
	panelNum = 0;
}

void LevelGauge::Update()
{
	Player* player = ActorManager::GetInstance()->GetPlayer();
	const int BONUS_COUNT_UINT = 5;
	level = player->GetBonusCount() / BONUS_COUNT_UINT;
	if (level <= 0)
		level = 1;
	panelNum = player->GetBonusCount();
}

void LevelGauge::Draw()
{
	Vector2 texSize = { 180,540 };
	Vector2 position = { texSize.x / 2,1080.0f - texSize.y + 37.0f };

	sp_levelText->DrawSprite("GamePlay_UI_Level" + std::to_string(level), position);

	position.y = 1080.0f;
	sp_gauge->SpriteSetTextureRect("GamePlay_UI_Levelgauge", 0.0f, texSize.y - (30.0f * panelNum + 25.0f), texSize.x, 30.0f * panelNum + 25.0f);
	sp_gauge->DrawSprite("GamePlay_UI_Levelgauge", position, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.5f, 1.0f });
	position.y = 1080.0f - (texSize.y / 2.0f);
	sp_frame->DrawSprite("GamePlay_UI_Leveledge", position, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.5f, 0.5f });
	sp_back->DrawSprite("GamePlay_UI_Levelback", position, 0.0f, { 1.0f,1.0f }, { 1,1,1,1 }, { 0.5f, 0.5f });
}
