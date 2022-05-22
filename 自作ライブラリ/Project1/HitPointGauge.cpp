#include "HitPointGauge.h"
#include "OBJLoader.h"

HitPointGauge::HitPointGauge(Vector3 arg_position, float arg_hitPoint, Vector3 arg_scale)
{
	position = arg_position;
	hitPoint = arg_hitPoint;
	scale = arg_scale;
	maxHitPoint = arg_hitPoint;
	maxScale = scale.x;

	position.y += 7;

	Create(OBJLoader::GetModel("box"));

	Initialize();
}

HitPointGauge::~HitPointGauge()
{
}

void HitPointGauge::Initialize()
{
	hitPoint = maxHitPoint;
	scale.x = maxScale;

	color = { 1,0.5f,0.2f,1 };
	rotation = { 0,0,0 };
	Object::Update();
}

void HitPointGauge::Update()
{
	
	Object::Update();
}

void HitPointGauge::Draw()
{
	Object::Draw();
}

void HitPointGauge::SetHitPoint(float arg_hitPoint)
{
	hitPoint = arg_hitPoint;
	scale.x = maxScale * (hitPoint / maxHitPoint);
}




