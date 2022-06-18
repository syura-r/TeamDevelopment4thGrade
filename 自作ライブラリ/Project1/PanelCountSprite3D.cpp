#include "PanelCountSprite3D.h"
#include "ActorManager.h"
#include "Player.h"

PanelCountSprite3D::PanelCountSprite3D(Vector3& parentPosition, const std::string& parentName, int& panelNum)
	:parentPosition(parentPosition), panelNum(panelNum)
{
	//画像の変更
	std::string objName = parentName;
	if (objName == "class Player")
	{
		iconTexName = "GamePlay_UI_Gettriangle_Player";
	}
	else
	{
		iconTexName = "GamePlay_UI_Gettriangle_Enemy";
	}

	icon_sp3D = new Sprite3D();
	for (int i = 0; i < digit; i++)
	{
		num_sp3D[i] = new Sprite3D();
	}

	Initialize();
}

PanelCountSprite3D::~PanelCountSprite3D()
{
	delete icon_sp3D;
	for (int i = 0; i < digit; i++)
	{
		delete num_sp3D[i];
	}
}

void PanelCountSprite3D::Initialize()
{
	position = parentPosition;

	for (int i = 0; i < digit; i++)
	{
		drawNum[i] = 0;
	}

}

void PanelCountSprite3D::Update()
{
	//所持数の各桁を求める
	DigitNum();

	//上にずらす
	const Vector3 parentAddPosition = { -1,4,0 };
	position = parentPosition + parentAddPosition;
}

void PanelCountSprite3D::Draw()
{
	//画像サイズ
	const Vector2 iconTexSize = { 432, 258 };
	const Vector2 numTexSize = { 47, 86 };
	//拡大率
	const Vector2 iconTexScale = { 0.1f, 0.1f };
	const Vector2 numTexScale = { 0.3f, 0.3f };

	Vector3 drawPosition = position;

	icon_sp3D->DrawSprite(iconTexName, drawPosition, 0.0f, iconTexScale);
	drawPosition.x += (iconTexSize.x * iconTexScale.x / 10) / 2.0f;

	for (int i = 0; i < std::to_string((int)panelNum).size(); i++)
	{
		drawPosition.x += (numTexSize.x * numTexScale.x / 10) / 2.0f;

		num_sp3D[i]->SpriteSetTextureRect("GamePlay_UI_Number", numTexSize.x * drawNum[i], 0, numTexSize.x, numTexSize.y);
		num_sp3D[i]->DrawSprite("GamePlay_UI_Number", drawPosition, 0.0f, numTexScale);

		const float spaceX = 0.5f;
		drawPosition.x += spaceX;
	}
}

void PanelCountSprite3D::DigitNum()
{
	std::string numStock = std::to_string((int)panelNum);

	//カンストした時はスキップ
	if (digit < numStock.size())
		return;

	for (int i = 0; i < numStock.size(); i++)
	{
		const int cord = 48;
		drawNum[i] = numStock[i] - cord;
	}
}
