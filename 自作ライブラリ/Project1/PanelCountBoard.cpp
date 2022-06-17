#include "PanelCountBoard.h"
#include "OBJLoader.h"
#include "ActorManager.h"
#include "Player.h"

PanelCountBoard::PanelCountBoard(Object* parentObject)
{
	this->parentObject = parentObject;
	std::string objName = parentObject->GetName();
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
	position = parentObject->GetPosition();
	SetBillboardType(BILLBOARD_TYPE::Y_AXIS);
	Object::Update();
}

void PanelCountBoard::Update()
{
	const Vector3 parentAddPosition = { 0,4,0 };
	position = parentObject->GetPosition() + parentAddPosition;

	//panelNum = ActorManager::GetInstance()->GetPlayer()->GetGottenPanel();

	Object::Update();
}
