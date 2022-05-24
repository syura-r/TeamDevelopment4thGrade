#include "Line.h"
#include "OBJLoader.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "LocusDef.h"

Line::Line(const Vector3& arg_startPos, float arg_angle, float arg_length, const Vector4& arg_color, const Vector3& arg_scale)
	:endPos(arg_startPos),
	 direction(LocusUtility::AngleToVector2(arg_angle)),
	 angle(arg_angle),
	 length(arg_length),
	 isDraw(true)
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
	endPos = position + direction * length;
}

void Line::Update()
{	
	scale.x = length;
	rotation = { 0,angle,0 };
	direction = LocusUtility::AngleToVector2(angle);
	endPos = position + direction * length;
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

Vector3 Line::GetDirection()
{
	return direction;
}

float Line::GetLength() const
{
	return length;
}

void Line::SetLength(const float arg_length)
{
	length = arg_length;
	endPos = position + direction * length;
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
