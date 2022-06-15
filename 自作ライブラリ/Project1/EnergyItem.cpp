#include "EnergyItem.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Field.h"

const float EnergyItem::RADIUS = 1.5f;

EnergyItem::EnergyItem(const Vector3& arg_position)
	:virtualityPlanePosition(arg_position),
	 preVirtualityPlanePosition(arg_position)
{
	Create(OBJLoader::GetModel("sphere"));

	appearTimer = new Timer(60);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("EnergyItem", this);

	Initialize();
}

EnergyItem::~EnergyItem()
{
	delete appearTimer;
	ActorManager::GetInstance()->DeleteObject(this);
}

void EnergyItem::Initialize()
{
	name = typeid(*this).name();		
	position = virtualityPlanePosition;
	scale = Vector3(0.0001f, 0.0001f, 0.0001f);
	color = { 0.8f, 0.8f, 0.8f, 1.0f };
	appearTimer->Initialize();
	Object::Update();
}

void EnergyItem::Update()
{
	preVirtualityPlanePosition = virtualityPlanePosition;
	if (!appearTimer->IsTime())
	{
		Appear();
	}

	if (IsAppeared())
	{
		SlidingDown();
	}

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));

	Object::Update();
}

void EnergyItem::Appear()
{
	appearTimer->Update();

	scale.x = scale.y = scale.z = Easing::Lerp(0, RADIUS, appearTimer->GetRate());

	if (appearTimer->IsTime())
	{
		color = { 0.3f, 0.3f, 1.0f, 1.0f };
	}
}

void EnergyItem::SlidingDown()
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.01f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
	StayOnRemainPanels();
}

void EnergyItem::StayInTheField()
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
		if (fabsf(cross) > 0.01f)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			Dead();
			return;
		}

		if (Vector2::Length(AO) < 0.01f || Vector2::Length(BO) < 0.01f)
		{
			Dead();
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
			Dead();
			return;
		}
	}
}

void EnergyItem::StayOnRemainPanels()
{
	if (dead)
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, 0.01f);

	if (piece)
	{
		Dead();
	}
}

Vector3 EnergyItem::GetVirtualityPlanePosition()
{
	return virtualityPlanePosition;
}

float EnergyItem::GetRadius()
{
	return RADIUS;
}

bool EnergyItem::IsAppeared()
{
	return appearTimer->IsTime();
}
