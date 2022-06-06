#include "Line.h"
#include "OBJLoader.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "LocusDef.h"
#include "ActorManager.h"
#include "Field.h"

Line::Line(const Vector3& arg_startPos, float arg_angle, float arg_length, const DirectX::XMFLOAT4& arg_color, const Vector3& arg_scale)
	:endPos(arg_startPos),
	 direction(LocusUtility::AngleToVector2(arg_angle)),
	 angle(arg_angle),
	 length(arg_length),
	 isDraw(true),
	 virtualityPlaneStartPos(arg_startPos),
	 virtualityPlaneEndPos(arg_startPos)
{
	position = arg_startPos;
	color = arg_color;
	scale = arg_scale;

	Create(OBJLoader::GetModel("obBox"));
	rotation = { 0,angle,0 };
	Object::Update();

	Initialize();
}

Line::~Line()
{
}

void Line::Initialize()
{	
	virtualityPlaneEndPos = virtualityPlaneStartPos + direction * length;	
	position = LocusUtility::RotateForFieldTilt(virtualityPlaneStartPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	endPos = LocusUtility::RotateForFieldTilt(virtualityPlaneEndPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
}

void Line::Update()
{	
	scale.x = length;
	rotation = Vector3(0, angle, 0);	
	UpdateRotation();
	direction = LocusUtility::AngleToVector2(angle);
	virtualityPlaneEndPos = virtualityPlaneStartPos + direction * length;
	position = LocusUtility::RotateForFieldTilt(virtualityPlaneStartPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	endPos = LocusUtility::RotateForFieldTilt(virtualityPlaneEndPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	Object::Update();
}

void Line::Draw()
{
	if (isDraw)
	{
		Object::Draw();
	}
}

void Line::AddLength(float arg_addSpeed)
{
	length += arg_addSpeed;
}

void Line::Move(Vector3 arg_movePos, float arg_angle)
{
	virtualityPlaneStartPos = arg_movePos;
	position = LocusUtility::RotateForFieldTilt(virtualityPlaneStartPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	angle = arg_angle;
}

Vector3 Line::GetStartPos()
{
	return position;
}

Vector3 Line::GetEndPos()
{
	return endPos;
}

Vector3 Line::GetDirection()
{
	return Vector3::Normalize(direction);
}

float Line::GetLength() const
{
	return length;
}

void Line::SetLength(const float arg_length)
{
	length = arg_length;
	virtualityPlaneEndPos = virtualityPlaneStartPos + direction * length;
	position = LocusUtility::RotateForFieldTilt(virtualityPlaneStartPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
	endPos = LocusUtility::RotateForFieldTilt(virtualityPlaneEndPos, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));
}

float Line::GetAngle() const
{
	return angle;
}

bool Line::IsDraw() const
{
	return isDraw;
}

void Line::ChangeIsDraw(const bool arg_isDraw)
{
	isDraw = arg_isDraw;
}

Vector3 Line::GetVirtualityPlaneStartPos() const
{
	return virtualityPlaneStartPos;
}

Vector3 Line::GetVirtualityPlaneEndPos() const
{
	return virtualityPlaneEndPos;
}

void Line::UpdateRotation()
{
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
	Vector3 fieldRot = ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt();
	XMMATRIX fieldRotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fieldRot.x), XMConvertToRadians(fieldRot.y), XMConvertToRadians(fieldRot.z));
	rotMat *= fieldRotMat;
	XMVECTOR rotQuat;
	XMMatrixDecompose(&XMVECTOR(), &rotQuat, &XMVECTOR(), rotMat);
	rotation = LocusUtility::ToEuler(quaternion(rotQuat.m128_f32[0], rotQuat.m128_f32[1], rotQuat.m128_f32[2], rotQuat.m128_f32[3]));
}
