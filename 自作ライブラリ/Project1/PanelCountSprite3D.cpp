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
		num_sp3D[i]->SetParent(icon_sp3D);
	}
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
	const Vector2 inSprite3dScale = { 0.1f, 0.1f };//Sprite3D内部で行われている分
	const Vector2 iconTexScale = { 0.1f, 0.1f };
	const Vector2 numTexScale = { 0.008f, 0.013f };

	//三角とバツ印部分
	icon_sp3D->DrawSprite(iconTexName, position, 0.0f, iconTexScale);

	//数字部分
	for (int i = 0; i < std::to_string((int)panelNum).size(); i++)
	{
		const float engineScale = 0.1f;

		Vector3 drawPosition = { 0,0,0 };
		//icon_sp3Dのアンカーポイント分ずらす
		const float iconHalfSizeX = (iconTexSize.x * iconTexScale.x) / 2.0f / 35.0f * engineScale;
		drawPosition.x += iconHalfSizeX;
		//num_sp3Dのアンカーポイント分ずらす
		const float numHarfSizeX = (numTexSize.x * numTexScale.x) / 2.0f * engineScale;
		drawPosition.x += numHarfSizeX * ((2 * i) + 1);

		num_sp3D[i]->SpriteSetTextureRect("GamePlay_UI_Number", numTexSize.x * drawNum[i], 0, numTexSize.x, numTexSize.y);
		num_sp3D[i]->DrawSprite("GamePlay_UI_Number", drawPosition, 0.0f, numTexScale);
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
