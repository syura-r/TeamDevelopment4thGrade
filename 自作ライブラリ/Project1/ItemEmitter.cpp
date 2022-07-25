#include "ItemEmitter.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Player.h"
#include "EnergyItem.h"
#include "Field.h"
#include "FieldPiece.h"
#include "PanelItem.h"
#include "FeverInItem.h"
#include "ActorManager.h"

ItemEmitter* ItemEmitter::instance = nullptr;

ItemEmitter::ItemEmitter()
	:energyItemTimer(new Timer(5 * 60))
{
}

ItemEmitter::~ItemEmitter()
{
	delete energyItemTimer;
}

ItemEmitter* ItemEmitter::GetInstance()
{
	if (!instance)
	{
		instance = new ItemEmitter();
	}
	return instance;
}

void ItemEmitter::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void ItemEmitter::Initialize()
{
	energyItemTimer->Reset();
}

void ItemEmitter::Update()
{
	energyItemTimer->Update();
	if (energyItemTimer->IsTime())
	{
		static const int EMIT_NUM = 2;
		for (int i = 0; i < EMIT_NUM; i++)
		{
			int n = ActorManager::GetInstance()->GetEnergyItems().size();
			if (n >= 30)
			{
				break;
			}

			//Vector3 emitPos = GetEnergyItemEmitPosition();
			Field* field = ActorManager::GetInstance()->GetFields()[0];
			FieldPiece* piece = nullptr;
			do
			{
				piece = field->GetRandomActivePiece();
			} while (IsClushedWithRemainItem(piece));

			EmitEnergyItem(piece->GetVirtualityPlanePosition() + Vector3(0, EnergyItem::GetRadius(), 0), RankEnergyItem::NORMAL, piece);
		}
	}

#ifdef _DEBUG
	if (Input::TriggerKey(DIK_N))
	{
		EmitPanelItem(ActorManager::GetInstance()->GetPlayer()->GetVirtualityPlanePosition());
	}
	if (Input::TriggerKey(DIK_L))
	{
		Field* field = ActorManager::GetInstance()->GetFields()[0];
		Player* player = ActorManager::GetInstance()->GetPlayer();
		EmitFeverInItem(player->GetVirtualityPlanePosition(), field->GetCuttingInfo(player)->ridingPiece);
	}
#endif // _DEBUG
}

bool ItemEmitter::IsClushedWithRemainItem(FieldPiece* arg_piece)
{
	auto energyItems = ActorManager::GetInstance()->GetEnergyItems();
	for (auto e : energyItems)
	{
		if (arg_piece == e->GetRidingPiece())
		{
			return true;
		}
	}

	auto feverInItems = ActorManager::GetInstance()->GetFeverInItems();
	for (auto f : feverInItems)
	{
		if (arg_piece == f->GetRidingPiece())
		{
			return true;
		}
	}

	return false;
}

void ItemEmitter::EmitEnergyItem(const Vector3& arg_pos, const RankEnergyItem arg_rank, FieldPiece* arg_piece)
{
	EnergyItem* item = new EnergyItem(arg_pos, arg_rank);
	item->SetRidingPiece(arg_piece);
	ObjectManager::GetInstance()->Add(item);	
	energyItemTimer->Reset();
}

void ItemEmitter::EmitPanelItem(const Vector3& arg_pos)
{
	Vector3 vel = GetRandomEmitPosition(1, 2);
	vel.y = 0;
	vel = vel.Normalize();

	PanelItem* item = new PanelItem(arg_pos, vel);
	ObjectManager::GetInstance()->Add(item);
}

void ItemEmitter::EmitFeverInItem(const Vector3& arg_pos, FieldPiece* arg_piece)
{
	if (IsClushedWithRemainItem(arg_piece))
	{
		return;
	}

	FeverInItem* item = new FeverInItem(arg_piece->GetVirtualityPlanePosition() + Vector3(0, FeverInItem::GetRadius(), 0));
	item->SetRidingPiece(arg_piece);
	ObjectManager::GetInstance()->Add(item);
}

Vector3 ItemEmitter::GetRandomEmitPosition(const int arg_min, const int arg_max)
{
	Vector3 returnVal;	
	
	float angle = rand() % 360;
	float range = rand() % (arg_max - arg_min) + arg_min;
	returnVal += LocusUtility::AngleToVector2(angle) * range;	

	return returnVal;
}

Vector3 ItemEmitter::GetEnergyItemEmitPosition()
{
	Vector3 returnVal;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	std::vector<FieldPiece*> gottenPieces = field->GetGottenPieces();
	bool b = true;

	do
	{
		returnVal = GetRandomEmitPosition(1, 38);
		for (auto p : gottenPieces)
		{
			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(returnVal - p->GetVirtualityPlanePosition())) <= FieldPiece::GetSize() + EnergyItem::GetRadius())
			{
				b = false;
				break;
			}
			else
			{
				b = true;
			}
		}
	} while (!b);

	returnVal.y += field->GetPosition().y + EnergyItem::GetRadius();
	return returnVal;
}
