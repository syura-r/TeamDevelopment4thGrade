#pragma once
#include "NumberSprite.h"

enum GAMEOBJECT_TYPE
{
	PLAYER,
	ENEMY,
};
class PanelCountUI
{
public:
	PanelCountUI(const GAMEOBJECT_TYPE type = GAMEOBJECT_TYPE::PLAYER);
	~PanelCountUI();

	void Initialize();
	void Update(const int panelNum);
	void Draw(const GAMEOBJECT_TYPE type = GAMEOBJECT_TYPE::PLAYER);

private:
	//�A�C�R��
	Sprite* icon_sprite;
	//�������\��
	NumberSprite* getPanel_sprite;
	float panelNum = 0;

	static int enemyCountNum;
	int enemyNum = 0;
};
