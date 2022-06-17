#include "PanelCountBoard.h"
#include "OBJLoader.h"
#include "ActorManager.h"
#include "Player.h"

PanelCountBoard::PanelCountBoard(Vector3& parentPosition, const std::string& parentName, int& panelNum)
	:parentPosition(parentPosition), panelNum(panelNum)
{
	std::string objName = parentName;
	if (objName == "class Player")
	{
		Create(OBJLoader::GetModel("getPanelBoard_Player"));
	}
	else
	{
		Create(OBJLoader::GetModel("getPanelBoard_Enemy"));
	}

	Initialize();
}

PanelCountBoard::~PanelCountBoard()
{
}

void PanelCountBoard::Initialize()
{
	position = parentPosition;

	for (int i = 0; i < digit; i++)
	{
		drawNum[i] = 0;
	}

	SetBillboardType(BILLBOARD_TYPE::Y_AXIS);
	Object::Update();
}

void PanelCountBoard::Update()
{
	//所持数の各桁を求める
	DigitNum();

	//上にずらす
	const Vector3 parentAddPosition = { 0,4,0 };
	position = parentPosition + parentAddPosition;

	Object::Update();
}

void PanelCountBoard::DigitNum()
{
	std::string numStock = std::to_string((int)panelNum);

	//カンストした時はスキップ
	if (digit < numStock.size())
	{
		return;
	}

	for (int i = 0; i < numStock.size(); i++)
	{
		const int cord = 48;
		drawNum[i] = numStock[i] - cord;
	}
}

//DrawでpanelNumの桁数分だけ数字を描画「例：12→120になっているため」
//桁数の求め方「std::to_string((int)panelNum).size()」
