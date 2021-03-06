#include "PanelItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"
#include "Audio.h"
#include "ObjectRegistType.h"

const float PanelItem::RADIUS = 1.5f;
const float PanelItem::BOUNCE_SPEED = 0.3f;

PanelItem::PanelItem(const Vector3& arg_position, const Vector3& arg_velocity)
	:virtualityPlanePosition(arg_position),
	 preVirtualityPlanePosition(arg_position),
	 bouncePower(0.5f)
{
	velocity = arg_velocity;
	Create(OBJLoader::GetModel("fieldPiece"));	

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::PANEL_ITEM);

	Initialize();
}

PanelItem::~PanelItem()
{	
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::PANEL_ITEM);
}

void PanelItem::Initialize()
{
	name = typeid(*this).name();
	virtualityPlanePosition.y += (RADIUS + 1.0f);
	preVirtualityPlanePosition = virtualityPlanePosition;
	position = virtualityPlanePosition;
	scale = Vector3(RADIUS * 1.5f, RADIUS *	1.5f, RADIUS * 1.5f);
	rotation = Vector3(0, 90, 0);
	color = { 0.5f, 1.0f, 0.4f, 1.0f };	
	Object::Update();
}

void PanelItem::Update()
{
	preVirtualityPlanePosition = virtualityPlanePosition;
	if (IsEndBounce())
	{
		SlidingDown();
	}
	else
	{
		Bounce();
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());

	Object::Update();
}

void PanelItem::Bounce()
{
	bouncePower -= 0.03f;
	virtualityPlanePosition.y += bouncePower;
	virtualityPlanePosition += velocity * BOUNCE_SPEED;
	StayInTheField();

	if (IsEndBounce())
	{
		Field* field = ActorManager::GetInstance()->GetFields()[0];
		virtualityPlanePosition.y = field->GetPosition().y + RADIUS;
		Audio::PlaySE("SE_TriangleLost", 0.5f * Audio::volume_se);
	}
}

void PanelItem::SlidingDown()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.01f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
	StayOnRemainPanels();
}

void PanelItem::StayInTheField()
{
	std::vector<Vector2> fieldEdges = Field::GetEdges();	

	for (int i = 0; i < fieldEdges.size(); i++)
	{
		Vector2 A = fieldEdges[i];
		Vector2 B = fieldEdges[(i + 1) % fieldEdges.size()];
		Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;
		Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - B;
		Vector2 AB = B - A;
		Vector2 normalAB = Vector2::Normalize(AB);

		//????????????????
		float cross = Vector2::Cross(AO, normalAB);
		if (cross < 0)
		{
			//Dead();
			float posY = virtualityPlanePosition.y;
			virtualityPlanePosition = preVirtualityPlanePosition;
			virtualityPlanePosition.y = posY;
			return;
		}

		if (fabsf(cross) > RADIUS * 2)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			//Dead();
			float posY = virtualityPlanePosition.y;
			virtualityPlanePosition = preVirtualityPlanePosition;
			virtualityPlanePosition.y = posY;
			return;
		}

		if (Vector2::Length(AO) < RADIUS * 2 || Vector2::Length(BO) < RADIUS * 2)
		{
			//Dead();
			float posY = virtualityPlanePosition.y;
			virtualityPlanePosition = preVirtualityPlanePosition;
			virtualityPlanePosition.y = posY;
			return;
		}

		//????????????
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			//Dead();
			float posY = virtualityPlanePosition.y;
			virtualityPlanePosition = preVirtualityPlanePosition;
			virtualityPlanePosition.y = posY;
			return;
		}
	}
}

void PanelItem::StayOnRemainPanels()
{
	if (dead)
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, 0.01f);

	if (piece)
	{
		ActorManager::GetInstance()->GetFields()[0]->ReviveGottenPanel(piece);
		Dead();
	}

	auto gottenPieces = field->GetGottenPieces();
	for (auto p : gottenPieces)
	{
		bool isFall = true;

		if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - p->GetVirtualityPlanePosition())) > RADIUS + FieldPiece::GetLowerTimeOffset())
		{
			continue;
		}

		auto points = p->GetPoints();
		for (int i = 0; i < points.size(); i++)
		{
			Vector2 A = points[i];
			Vector2 B = points[(i + 1) % points.size()];
			Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;			
			Vector2 AB = B - A;
			Vector2 normalAB = Vector2::Normalize(AB);

			//????????????????
			float cross = Vector2::Cross(AO, normalAB);
			if (cross < 0)
			{
				isFall = false;
				break;
			}
		}

		if (isFall)
		{
			ActorManager::GetInstance()->GetFields()[0]->ReviveGottenPanel(p);
			Dead();
			return;
		}
	}
}

Vector3 PanelItem::GetVirtualityPlanePosition()
{
	return virtualityPlanePosition;
}

bool PanelItem::IsEndBounce()
{
	//Vector3 A = {};
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	//A = field->GetPosition().y + RADIUS;
	return virtualityPlanePosition.y <= field->GetPosition().y + RADIUS;
}

float PanelItem::GetRadius()
{
	return RADIUS;
}
