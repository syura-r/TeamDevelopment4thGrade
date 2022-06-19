#include "PanelCountUI.h"
#include "Vector.h"

int PanelCountUI::enemyCountNum = 0;

PanelCountUI::PanelCountUI(const GAMEOBJECT_TYPE type)
{
	icon_sprite = new Sprite();
	getPanel_sprite = new NumberSprite(panelNum);

	if (type == GAMEOBJECT_TYPE::ENEMY)
	{
		enemyCountNum++;
		enemyNum = enemyCountNum;
	}
}

PanelCountUI::~PanelCountUI()
{
	delete icon_sprite;
	delete getPanel_sprite;
	enemyCountNum = 0;
}

void PanelCountUI::Initialize()
{
	panelNum = 0;
}

void PanelCountUI::Update(const int panelNum)
{
	this->panelNum = panelNum;
}

void PanelCountUI::Draw(const GAMEOBJECT_TYPE type)
{
	//アイコン画像サイズ
	const Vector2 iconTexSize = { 141.0f, 86.0f };
	//数字画像サイズ
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//画面サイズ
	const Vector2 windowSize = { 1920.0f, 1080.0f };
	//余白
	const Vector2 spaceSize = { 50.0f, 30.0f };

	Vector2 position = { (iconTexSize.x / 2) + (numberTexSize.x * 2) + spaceSize.x, windowSize.y - (iconTexSize.y / 2) - spaceSize.y };
	Vector2 iconScale = { 0.3f, 0.3f };
	std::string iconTexName = "GamePlay_UI_Gettriangle_Player";

	//敵用
	if (type == GAMEOBJECT_TYPE::ENEMY)
	{
		position.x = windowSize.x - ((iconTexSize.x / 2) + (numberTexSize.x * 3)) - spaceSize.x;
		position.y -= iconTexSize.y * (enemyNum - 1);
		iconTexName = "GamePlay_UI_Gettriangle_Enemy";
	}

	icon_sprite->DrawSprite(iconTexName, position, 0.0f, iconScale);
	position.x += (iconTexSize.x / 2) + ((numberTexSize.x / 2) * 3);
	getPanel_sprite->Draw(3, "GamePlay_UI_Number", position);
}
