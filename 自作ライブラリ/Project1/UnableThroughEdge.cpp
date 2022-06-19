#include "UnableThroughEdge.h"
#include "OBJLoader.h"
#include "Field.h"
#include "FieldPiece.h"
#include "ActorManager.h"

const float UnableThroughEdge::WEIGHT = 2.5f;
const float UnableThroughEdge::HEIGHT_MAGNIFICATION = 1.5f;

UnableThroughEdge::UnableThroughEdge(const Vector3& arg_position, const float arg_emitHeight)
	:virtualityPlanePosition(arg_position),
	 preVirtualityPlanePosition(arg_position),
	 fallSpeed(0.5f)
{
	virtualityPlanePosition.y += arg_emitHeight;
	
	Create(OBJLoader::GetModel("fieldEdge"));

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("UnableThroughEdge", this);

	Initialize();
}

UnableThroughEdge::~UnableThroughEdge()
{
	ActorManager::GetInstance()->DeleteObject(this);
}

void UnableThroughEdge::Initialize()
{
	scale = Vector3(Field::GetRadius(), 6, Field::GetRadius());
	color = { 0.3f, 0.3f, 0.3f, 0.3f };	
	Object::Update();
}

void UnableThroughEdge::Update()
{
	preVirtualityPlanePosition = virtualityPlanePosition;

	rotation = Vector3(0, 0, 0);

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

void UnableThroughEdge::Draw()
{
	Object::Draw();
}

void UnableThroughEdge::FallDown()
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

Vector3 UnableThroughEdge::GetVirtualityPlanePosition() const
{
	return virtualityPlanePosition;
}

float UnableThroughEdge::GetWeight()
{
	return WEIGHT;
}

float UnableThroughEdge::GetHeightMagnification()
{
	return HEIGHT_MAGNIFICATION;
}

bool UnableThroughEdge::IsEndFallDown() const
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	return virtualityPlanePosition.y <= field->GetPosition().y + HEIGHT_MAGNIFICATION;
}