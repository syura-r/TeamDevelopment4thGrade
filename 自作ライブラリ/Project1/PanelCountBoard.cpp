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
	//�������̊e�������߂�
	DigitNum();

	//��ɂ��炷
	const Vector3 parentAddPosition = { 0,4,0 };
	position = parentPosition + parentAddPosition;

	Object::Update();
}

void PanelCountBoard::DigitNum()
{
	std::string numStock = std::to_string((int)panelNum);

	//�J���X�g�������̓X�L�b�v
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

//Draw��panelNum�̌���������������`��u��F12��120�ɂȂ��Ă��邽�߁v
//�����̋��ߕ��ustd::to_string((int)panelNum).size()�v
