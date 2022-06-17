#include "FieldPiece.h"
#include "Field.h"
#include "LocusDef.h"
#include "OBJLoader.h"
#include "ActorManager.h"

const float FieldPiece::SIZE = 7.5f / 0.866f / 2.0f;
const float FieldPiece::SIDEWAYS_LENGTH = 0.866f * SIZE;
const float FieldPiece::LOWER_TIME_OFFSET = 1.0f * SIZE;
const float FieldPiece::UPPER_TIME_OFFSET = 0.5f * SIZE;
const float FieldPiece::FULL_OFFSET = LOWER_TIME_OFFSET + UPPER_TIME_OFFSET;
const float FieldPiece::WEIGHT = 1.0f;
std::vector<Vector2> FieldPiece::basePoints = std::vector<Vector2>();

FieldPiece::FieldPiece(const Vector3& arg_position, const PieceDirection arg_direction)
	:virtualityPlanePosition(arg_position),
	 dir(arg_direction),
	 isActive(true)
{
	SetPoints();
	position = virtualityPlanePosition;

	Create(OBJLoader::GetModel("fieldPiece"));

	Initialize();
}

FieldPiece::~FieldPiece()
{
}

void FieldPiece::Initialize()
{
	rotation = Vector3(0, 0, 0);
	if (dir == PieceDirection::Upper)
	{
		rotation.y = 180;
	}
	scale = Vector3(SIZE, 1, SIZE);
	Object::Update();
}

void FieldPiece::Update()
{
	if (dir == PieceDirection::Lower)
	{
		rotation = Vector3(0, 0, 0);
	}
	else
	{
		rotation = Vector3(0, 180, 0);
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
	Vector3 fieldRot = field->GetAngleTilt();
	XMMATRIX fieldRotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fieldRot.x), XMConvertToRadians(fieldRot.y), XMConvertToRadians(fieldRot.z));
	rotMat *= fieldRotMat;
	XMVECTOR rotQuat;
	XMMatrixDecompose(&XMVECTOR(), &rotQuat, &XMVECTOR(), rotMat);
	rotation = LocusUtility::ToEuler(quaternion(rotQuat.m128_f32[0], rotQuat.m128_f32[1], rotQuat.m128_f32[2], rotQuat.m128_f32[3]));

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());

	Object::Update();
}

void FieldPiece::Draw()
{
	if (!isActive)
	{
		return;
	}

	color = { 0.3f, 0.3f, 0.3f, 1.0f };
	if (ActorManager::GetInstance()->GetFields()[0]->GetPlayerRidingPiece() == this)
	{
		color = { 0.8f, 0.1f, 0.1f, 1.0f };
	}	

	Object::Draw();
}

bool FieldPiece::IsRidden(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius)
{
	for (int i = 0; i < points.size(); i++)
	{
		if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - arg_pos)) > SIZE + arg_radius)
		{
			continue;
		}

		Vector2 A = points[i];
		Vector2 B = points[(i + 1) % points.size()];
		Vector2 AO = LocusUtility::Dim3ToDim2XZ(arg_pos) - A;
		Vector2 BO = LocusUtility::Dim3ToDim2XZ(arg_pos) - B;
		Vector2 AB = B - A;
		Vector2 normalAB = Vector2::Normalize(AB);

		//今当たっているか
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > arg_radius)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{						
			return true;
		}

		if (Vector2::Length(AO) < arg_radius || Vector2::Length(BO) < arg_radius)
		{
			return true;
		}

		//通り過ぎたか
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(arg_pos);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(arg_prePos);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			return true;
		}
	}

	return false;
}

float FieldPiece::GetSize()
{
	return SIZE;
}

float FieldPiece::GetSidewaysLength()
{
	return SIDEWAYS_LENGTH;
}

float FieldPiece::GetLowerTimeOffset()
{
	return LOWER_TIME_OFFSET;
}

float FieldPiece::GetUpperTimeOffset()
{
	return UPPER_TIME_OFFSET;
}

float FieldPiece::GetFullOffset()
{
	return FULL_OFFSET;
}

Vector3 FieldPiece::GetVirtualityPlanePosition() const
{
	return virtualityPlanePosition;
}

float FieldPiece::GetWeight()
{
	return WEIGHT;
}

PieceDirection FieldPiece::GetPieceDirection() const
{
	return dir;
}

bool FieldPiece::IsActive()
{
	return isActive;
}

void FieldPiece::ChangeIsActive(const bool arg_isActive)
{
	isActive = arg_isActive;
}

std::vector<Vector2>& FieldPiece::GetPoints()
{
	return points;
}

void FieldPiece::SetPoints()
{
	if (basePoints.empty())
	{
		basePoints.push_back(Vector2( 0.000f, -LOWER_TIME_OFFSET));
		basePoints.push_back(Vector2(-SIDEWAYS_LENGTH,  UPPER_TIME_OFFSET));
		basePoints.push_back(Vector2( SIDEWAYS_LENGTH,  UPPER_TIME_OFFSET));		
	}

	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dir == PieceDirection::Lower)
		{
			points.push_back(basePoints[i]);
		}
		else
		{
			points.push_back(basePoints[i] * -1);
		}

		points[i] += LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
	}
}
