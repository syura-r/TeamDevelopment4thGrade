#include "HitPointGauge.h"
#include "OBJLoader.h"

HitPointGauge::HitPointGauge(Vector3 arg_position, Vector3 arg_scale, float arg_hitPoint)
	:hitPoint(arg_hitPoint),
	 maxHitPoint(arg_hitPoint),
	 maxScale(arg_scale.x)
{
	position = arg_position;
	position.y += 7;
	scale = arg_scale;	

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




