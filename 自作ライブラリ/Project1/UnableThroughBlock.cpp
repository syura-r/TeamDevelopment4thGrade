#include "UnableThroughBlock.h"
#include "OBJLoader.h"
#include "Field.h"
#include "FieldPiece.h"
#include "ActorManager.h"

const float UnableThroughBlock::WEIGHT = 2.5f;
const float UnableThroughBlock::HEIGHT_MAGNIFICATION = 3.0f;

UnableThroughBlock::UnableThroughBlock(const Vector3& arg_position, const float arg_emitHeight, FieldPiece* arg_piece)
	:virtualityPlanePosition(arg_position),
	preVirtualityPlanePosition(arg_position),
	dir(arg_piece->GetPieceDirection()),
	parentPiece(arg_piece),
	fallSpeed(0.5f)
{
	virtualityPlanePosition.y += arg_emitHeight;

	points = parentPiece->GetPoints();
	Create(OBJLoader::GetModel("fieldPiece"));

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("UnableThroughBlock", this);

	Initialize();
}

UnableThroughBlock::~UnableThroughBlock()
{
	ActorManager::GetInstance()->DeleteObject(this);
}

void UnableThroughBlock::Initialize()
{
	scale = Vector3(FieldPiece::GetSize(), 30, FieldPiece::GetSize());
	color = { 0.3f, 0.3f, 0.3f, 0.3f };
	if (dir == PieceDirection::Lower)
	{
		rotation = Vector3(0, 0, 0);
	}
	else
	{
		rotation = Vector3(0, 180, 0);
	}
	Object::Update();
}

void UnableThroughBlock::Update()
{
	preVirtualityPlanePosition = virtualityPlanePosition;

	if (dir == PieceDirection::Lower)
	{
		rotation = Vector3(0, 0, 0);
	}
	else
	{
		rotation = Vector3(0, 180, 0);
	}

	if (IsEndFallDown())
	{
		Field* field = ActorManager::GetInstance()->GetFields()[0];

		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
		Vector3 fieldRot = field->GetAngleTilt();
		XMMATRIX fieldRotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fieldRot.x), XMConvertToRadians(fieldRot.y), XMConvertToRadians(fieldRot.z));
		rotMat *= fieldRotMat;
		XMVECTOR rotQuat;
		XMMatrixDecompose(&XMVECTOR(), &rotQuat, &XMVECTOR(), rotMat);
		rotation = LocusUtility::ToEuler(quaternion(rotQuat.m128_f32[0], rotQuat.m128_f32[1], rotQuat.m128_f32[2], rotQuat.m128_f32[3]));

		position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	}
	else
	{
		FallDown();
	}

	Object::Update();
}

void UnableThroughBlock::Draw()
{
	Object::Draw();
}

void UnableThroughBlock::FallDown()
{
	virtualityPlanePosition.y -= fallSpeed;
	position = virtualityPlanePosition;

	if (IsEndFallDown())
	{
		Field* field = ActorManager::GetInstance()->GetFields()[0];
		virtualityPlanePosition.y = field->GetPosition().y + HEIGHT_MAGNIFICATION;
		position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	}
}

Vector3 UnableThroughBlock::GetVirtualityPlanePosition() const
{
	return virtualityPlanePosition;
}

PieceDirection UnableThroughBlock::GetPieceDirection() const
{
	return dir;
}

std::vector<Vector2>& UnableThroughBlock::GetPoints()
{
	return points;
}

float UnableThroughBlock::GetWeight()
{
	return WEIGHT;
}

float UnableThroughBlock::GetHeightMagnification()
{
	return HEIGHT_MAGNIFICATION;
}

bool UnableThroughBlock::IsEndFallDown() const
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	return virtualityPlanePosition.y <= field->GetPosition().y + HEIGHT_MAGNIFICATION;
}
