#include "TestBoss.h"
#include "OBJLoader.h"
#include "ObjectManager.h"

TestBoss::TestBoss(Vector3 arg_position, float arg_hitPoint)
{
	position = arg_position;
	hitPoint = arg_hitPoint;

	Create(OBJLoader::GetModel("box"));

	hpGauge = new HitPointGauge(position, hitPoint, { 30,1,1 });
	ObjectManager::GetInstance()->Add(hpGauge);

	Initialize();
}

TestBoss::~TestBoss()
{
}

void TestBoss::Initialize()
{
	color = { 1,0.1f,0.2f,1 };//
	rotation = { 0,0,0 };//
	scale = { 5,5,5 };
	Object::Update();

	
}

void TestBoss::Update()
{
	Object::Update();
}

void TestBoss::Draw()
{
	Object::Draw();
}

void TestBoss::Reset()
{
}

void TestBoss::Damage(float arg_value)
{
	hitPoint -= arg_value;
	hpGauge->SetHitPoint(hitPoint);
}
