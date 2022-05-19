#include "Line.h"
#include "OBJLoader.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "LocusDef.h"

Line::Line(Vector3 arg_startPos, float arg_angle, float arg_length,Vector4 arg_color)
{
	
	this->angle = arg_angle;
	this->length = arg_length;
	position = arg_startPos;
	isDraw = true;

	Create(OBJLoader::GetModel("obBox"));
	color = arg_color;
	rotation = { 0,angle,0 };
	Object::Update();

	Initialize();
}

Line::~Line()
{
	
}

void Line::Initialize()
{
	DirectX::XMStoreFloat3(&velocity, LocusUtility::AngleToVector2(angle));
	endPos = position + velocity * length;

}

void Line::Update()
{
	scale = { length,0.5,0.5 };
	rotation = { 0,angle,0 };
	DirectX::XMStoreFloat3(&velocity, LocusUtility::AngleToVector2(angle));
	endPos = position + velocity * length;
	Object::Update();
}

void Line::Draw()
{
	if (isDraw)
	{
		Object::Draw();
	}
}

void Line::Reset()
{
}

void Line::AddLength(float arg_addSpeed)
{
	length += arg_addSpeed;
}

void Line::Move(Vector3 arg_movePos, float arg_angle)
{
	position = arg_movePos;
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

Vector3 Line::GetVelocity()
{
	return velocity;
}

float Line::GetLength() const
{
	return length;
}

void Line::SetLength(const float arg_length)
{
	length = arg_length;
	endPos = position + velocity * length;
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
