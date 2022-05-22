#include "TestBoss.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ActorManager.h"
#include "BossMissile.h"
#include "BossRangeAttack.h"

TestBoss::TestBoss(Vector3 arg_position, float arg_hitPoint)
{
	position = arg_position;
	maxHitPoint = arg_hitPoint;
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
	for (int i = 0; i < rangeAttacks.size(); i++)
	{
		rangeAttacks[i]->Dead();
	}
}

void TestBoss::Initialize()
{
	state = ActionState::Wait;
	hitPoint = maxHitPoint;
	attackCount = 0;
	actionTimer->Initialize();
	for (int i = 0; i < missiles.size(); i++)
	{
		missiles[i]->Dead();
	}
	for (int i = 0; i < rangeAttacks.size(); i++)
	{
		rangeAttacks[i]->Dead();
	}
	missiles.clear();
	rangeAttacks.clear();
	hpGauge->Initialize();

	color = { 1,0.1f,0.2f,1 };//
	rotation = { 0,0,0 };//
	scale = { 5,5,5 };
	Object::Update();
}

void TestBoss::Update()
{
	if (!IsAlive() || !ActorManager::GetInstance()->GetPlayer()->IsAlive())
	{
		return;
	}

	actionTimer->Update();
	if (actionTimer->IsTime())
	{
		switch (state)
		{
		case ActionState::Wait:
			actionTimer->Reset();
			if (attackCount % 2 != 0)
			{
				state = ActionState::ShotMissile;
			}
			else
			{
				ExpandRangeAttack();
				state = ActionState::ExpandRangeAttack;
			}
			attackCount++;
			break;
		case ActionState::ShotMissile:
			ShotMissile();
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		case ActionState::ExpandRangeAttack:
			actionTimer->Reset();
			state = ActionState::CoolAfterRangAttack;
			break;
		case ActionState::CoolAfterRangAttack:
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		default:
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		}
	}

	if (state == ActionState::ShotMissile)
	{
		Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();
		float angle = LocusUtility::Vector2ToAngle(Vector3::Normalize(playerPos - position));
		rotation = Vector3(0, angle, 0);
	}
	CheckMissilesDuration();
	CheckRangeAttacksDuration();

	Object::Update();
}

void TestBoss::Draw()
{
	ImGui::Begin("boss");
	ImGui::Text("%f", hitPoint);
	ImGui::End();
	Object::Draw();
}

void TestBoss::Reset()
{
}

void TestBoss::Damage(float arg_value)
{
	hitPoint -= arg_value;	

	if (hitPoint < 0)
	{
		hitPoint = 0;
	}
	
	hpGauge->SetHitPoint(hitPoint);
}

bool TestBoss::IsAlive()
{
	return hitPoint > 0;
}

std::vector<BossMissile*>& TestBoss::GetMissiles()
{
	return missiles;
}

std::vector<BossRangeAttack*>& TestBoss::GetRangeAttacks()
{
	return rangeAttacks;
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

void TestBoss::ExpandRangeAttack()
{
	Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();
	BossRangeAttack* rangeAttack = new BossRangeAttack(Vector3(playerPos.x, -5, 0), Vector3(10, 0.2f, 94), 360);
	rangeAttacks.push_back(rangeAttack);
	ObjectManager::GetInstance()->Add(rangeAttack);
}

void TestBoss::CheckRangeAttacksDuration()
{
	for (int i = 0; i < rangeAttacks.size(); i++)
	{		
		if (rangeAttacks[i]->IsFinish())
		{
			rangeAttacks[i]->Dead();
		}
	}

	for (auto itr = rangeAttacks.begin(); itr != rangeAttacks.end();)
	{
		if ((*itr)->IsDead())
		{
			itr = rangeAttacks.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}
