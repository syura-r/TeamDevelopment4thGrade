#include "TestBoss.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ActorManager.h"
#include "BossMissile.h"

TestBoss::TestBoss(Vector3 arg_position, float arg_hitPoint)
{
	position = arg_position;
	hitPoint = arg_hitPoint;

	Create(OBJLoader::GetModel("box"));

	hpGauge = new HitPointGauge(position, hitPoint, { 30,1,1 });
	ObjectManager::GetInstance()->Add(hpGauge);

	state = ActionState::Wait;
	actionTimer = new Timer(180);

	Initialize();
}

TestBoss::~TestBoss()
{
	delete actionTimer;
	for (int i = 0; i < missiles.size(); i++)
	{
		missiles[i]->Dead();
	}
}

void TestBoss::Initialize()
{
	state = ActionState::Wait;
	actionTimer->Initialize();
	missiles.clear();

	color = { 1,0.1f,0.2f,1 };//
	rotation = { 0,0,0 };//
	scale = { 5,5,5 };
	Object::Update();
}

void TestBoss::Update()
{
	actionTimer->Update();
	if (actionTimer->IsTime())
	{
		switch (state)
		{
		case ActionState::Wait:
			actionTimer->Reset();
			state = ActionState::Shot;
			break;
		case ActionState::Shot:
			ShotMissile();
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		default:
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		}
	}

	if (state == ActionState::Shot)
	{
		Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();
		float angle = LocusUtility::Vector2ToAngle(Vector3::Normalize(playerPos - position));
		rotation = Vector3(0, angle, 0);
	}
	CheckMissilesDuration();

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

std::vector<BossMissile*>& TestBoss::GetMissiles()
{
	return missiles;
}

void TestBoss::ShotMissile()
{
	Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();

	Vector3 missileVel = playerPos - position;
	missileVel.Normalize();
	missileVel.y = 0;

	BossMissile* missile = new BossMissile(position, missileVel);
	missiles.push_back(missile);
	ObjectManager::GetInstance()->Add(missile);
}

void TestBoss::CheckMissilesDuration()
{
	for (int i = 0; i < missiles.size(); i++)
	{
		Vector3 missilePos = missiles[i]->GetPosition();
		if (missilePos.x > 60 || missilePos.x < -60 || missilePos.z < -60)
		{
			missiles[i]->Dead();
		}
	}

	for (auto itr = missiles.begin(); itr != missiles.end();)
	{		
		if ((*itr)->IsDead())
		{
			itr = missiles.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}
