#include "BossMissile.h"
#include "OBJLoader.h"

BossMissile::BossMissile(const Vector3& arg_position, const Vector3& arg_velocity)
{
	position = arg_position;
	velocity = arg_velocity;
	speed = 1.0f;

	Create(OBJLoader::GetModel("sphere"));

	Initialize();
}

BossMissile::~BossMissile()
{
}

void BossMissile::Initialize()
{
	color = { 1,0.1f,0.2f,1 };//	
	scale = { 3,3,3 };
	Object::Update();
}

void BossMissile::Update()
{
	position += velocity * speed;
	Object::Update();
}

void BossMissile::Draw()
{
	Object::Draw();
}
