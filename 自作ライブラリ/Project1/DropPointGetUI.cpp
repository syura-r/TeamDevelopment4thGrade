#include "DropPointGetUI.h"
#include "Easing.h"

int DropPointGetUI::enemyCount = 0;

DropPointGetUI::DropPointGetUI(Vector3& parentPosition, const std::string& parentName)
	:parentPosition(parentPosition)
{
	//âÊëúÇÃïœçX
	std::string objName = parentName;
	if (objName == "class Player")
	{
		texName = "GamePlay_UI_Playerpoint";
	}
	else
	{
		if (enemyCount % 2)
			texName = "GamePlay_UI_Enemy2point";
		else
			texName = "GamePlay_UI_Enemypoint";

		enemyCount++;
	}
}

DropPointGetUI::~DropPointGetUI()
{
	for (int i = 0; i < ui.size(); i++)
	{
		delete ui[i]->sprite;
	}
	ui.clear();
}

void DropPointGetUI::Initialize()
{
	ui.clear();
}

void DropPointGetUI::Update()
{
	for (int i = 0; i < ui.size(); i++)
	{
		if (ui[i]->isActive)
		{
			const int limitTime = 45;

			//â∫ç~
			const float positionUpSpeed = 0.1f;
			ui[i]->position.y -= positionUpSpeed;

			//îñÇ≠
			ui[i]->alpha = Easing::EaseInQuart(1.0f, 0.6f, limitTime, ui[i]->moveTimer);

			ui[i]->moveTimer++;
			if (ui[i]->moveTimer >= limitTime)
			{
				ui[i]->Initialize(parentPosition, addPositionY);
			}
		}
		else
			ui[i]->position = parentPosition + XMFLOAT3{ 0, addPositionY, 0 };
	}
}

void DropPointGetUI::Draw()
{
	for (int i = 0; i < ui.size(); i++)
	{
		if (ui[i]->isActive)
			ui[i]->sprite->DrawSprite(texName, ui[i]->position, 0.0f, { scale,scale }, { 1,1,1,ui[i]->alpha });
	}
}

void DropPointGetUI::PointGet()
{
	//égÇ¡ÇƒÇ¢Ç»Ç¢ÉÇÉmÇ™Ç†ÇÍÇŒÇªÇÍÇégÇ§
	for (int i = 0; i < ui.size(); i++)
	{
		if (!ui[i]->isActive)
		{
			ui[i]->isActive = true;
			return;
		}
	}

	OneUI* addUI = new OneUI();
	addUI->sprite = new Sprite3D();
	addUI->Initialize(parentPosition, addPositionY);
	addUI->isActive = true;

	ui.push_back(addUI);
}

void DropPointGetUI::OneUI::Initialize(const Vector3& parentPosition, const float addPositionY)
{
	position = parentPosition + XMFLOAT3{ 0, addPositionY, 0 };
	alpha = 1.0f;
	isActive = false;
	moveTimer = 0;
}
