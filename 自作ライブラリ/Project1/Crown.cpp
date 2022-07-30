#include "Crown.h"
#include "OBJLoader.h"
#include "Easing.h"

Crown::Crown()
{
	object = Object3D::Create(OBJLoader::GetModel("Crown"), position, scale, rotation, color);
}

Crown::~Crown()
{
	delete object;
}

void Crown::Initialize(const float arg_addPositionY)
{
	position = { 0.0f,0.0f,0.0f };
	rotation = { 0.0f,0.0f,0.0f };

	isTop = false;
	addPositionY = 0.0f;
	addPositionY_base = arg_addPositionY;

	isPositionUp = false;
	positionUpEasingCount = 0;
}

void Crown::Update(const int ranking, const Vector3& arg_position)
{
	isTop = (ranking == 1);

	MovePosition();
	position = arg_position + Vector3(0.0f, addPositionY, 0.0f);

	const float rotSpeed = 1.0f;
	rotation.y += rotSpeed;

	object->Update();
}

void Crown::Draw()
{
	if (!isTop)
	{
		return;
	}

	PipelineState::SetPipeline("BasicObj");
	object->Draw();
}

void Crown::MovePosition()
{
	const int limit = 120;
	if (isPositionUp)
	{
		addPositionY = Easing::EaseOutSine(addPositionY_down, addPositionY_up, limit, positionUpEasingCount);
	}
	else
	{
		addPositionY = Easing::EaseOutSine(addPositionY_up, addPositionY_down, limit, positionUpEasingCount);
	}
	positionUpEasingCount++;
	if (positionUpEasingCount >= limit)
	{
		positionUpEasingCount = 0;
		isPositionUp = !isPositionUp;
	}

	addPositionY += addPositionY_base;
}
