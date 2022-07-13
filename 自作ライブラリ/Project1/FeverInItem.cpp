#include "FeverInItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"
#include "ObjectRegistType.h"

const float FeverInItem::RADIUS = 1.5f;

FeverInItem::FeverInItem(const Vector3& arg_position)
	:virtualityPlanePosition(arg_position),
	 preVirtualityPlanePosition(arg_position),
	 appearTimer(new Timer(60)),
	 ridingPiece(nullptr)
{
	Create(OBJLoader::GetModel("sphere"));
	SetBillboardType(BILLBOARD_TYPE::Y_AXIS);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::FEVERIN_ITEM);

	Initialize();
}

FeverInItem::~FeverInItem()
{
	delete appearTimer;
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::FEVERIN_ITEM);
}

void FeverInItem::Initialize()
{
	name = typeid(*this).name();
	position = virtualityPlanePosition;
	scale = Vector3(0.0001f, 0.0001f, 0.0001f);
	color = { 0.4f, 0.4f, 0.4f, 1.0f };
	rotation = Vector3(0, 90, 0);
	appearTimer->Initialize();
	Object::Update();
}

void FeverInItem::Update()
{
	preVirtualityPlanePosition = virtualityPlanePosition;
	if (!appearTimer->IsTime())
	{
		Appear();
	}

	StayOnRemainPanels();

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());

	Object::Update();
}

void FeverInItem::Appear()
{
	appearTimer->Update();

	scale.x = scale.y = scale.z = Easing::Lerp(0, RADIUS, appearTimer->GetRate());

	if (appearTimer->IsTime())
	{
		color = { 0.9f, 0.9f, 0.9f, 1.0f };
	}
}

void FeverInItem::StayInTheField()
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

		//¡“–‚½‚Á‚Ä‚¢‚é‚©
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > RADIUS * 2)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			//Dead();
			virtualityPlanePosition = preVirtualityPlanePosition;
			return;
		}

		if (Vector2::Length(AO) < RADIUS * 2 || Vector2::Length(BO) < RADIUS * 2)
		{
			//Dead();
			virtualityPlanePosition = preVirtualityPlanePosition;
			return;
		}

		//’Ê‚è‰ß‚¬‚½‚©
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			//Dead();
			virtualityPlanePosition = preVirtualityPlanePosition;
			return;
		}
	}
}

void FeverInItem::StayOnRemainPanels()
{
	if (dead)
	{
		return;
	}

	if (!ridingPiece->IsActive())
	{
		Dead();
	}
	return;

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, 0.01f);

	if (piece)
	{
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

			//¡“–‚½‚Á‚Ä‚¢‚é‚©
			float cross = Vector2::Cross(AO, normalAB);
			if (cross < 0)
			{
				isFall = false;
				break;
			}
		}

		if (isFall)
		{
			Dead();
			return;
		}
	}
}

Vector3 FeverInItem::GetVirtualityPlanePosition()
{
	return virtualityPlanePosition;
}

float FeverInItem::GetRadius()
{
	return RADIUS;
}

bool FeverInItem::IsAppeared()
{
	return appearTimer->IsTime();
}

FieldPiece* FeverInItem::GetRidingPiece() const
{
	return ridingPiece;
}

void FeverInItem::SetRidingPiece(FieldPiece* arg_piece)
{
	ridingPiece = arg_piece;
}
