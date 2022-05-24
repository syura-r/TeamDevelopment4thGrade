#include "TestBoss.h"
#include "OBJLoader.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ActorManager.h"
#include "BossMissile.h"
#include "BossRangeAttack.h"

const float INTERVAL_ACTIONTIMER = 180.0f;

TestBoss::TestBoss(Vector3 arg_position, float arg_hitPoint)
	:hitPoint(arg_hitPoint),
	 maxHitPoint(arg_hitPoint),
	 hpGauge(new HitPointGauge(arg_position, Vector3(30, 1, 1), arg_hitPoint)),
	 state(ActionState::Wait),
	 attackCount(0),
	 actionTimer(new Timer(INTERVAL_ACTIONTIMER))
{
	position = arg_position;

	Create(OBJLoader::GetModel("box"));
	
	ObjectManager::GetInstance()->Add(hpGauge);	

	missiles.clear();
	rangeAttacks.clear();

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

	color = { 1,0.1f,0.2f,1 };
	rotation = { 0,0,0 };
	scale = { 5,5,5 };
	Object::Update();
}

void TestBoss::Update()
{
	//ゲームの終了判定
	//プレイヤーか自身のどちらかのHPがなくなったら
	//残りの処理を飛ばす
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
			//交互にミサイルと範囲攻撃を実行
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
			state = ActionState::CoolAfterRangeAttack;
			break;
		case ActionState::CoolAfterRangeAttack:
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		default:
			actionTimer->Reset();
			state = ActionState::Wait;
			break;
		}
	}

	//ミサイル発射モーション時は目線をPlayerに合わせる
	if (state == ActionState::ShotMissile)
	{
		Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();
		float angle = LocusUtility::Vector2ToAngle(Vector3::Normalize(playerPos - position));
		rotation = Vector3(0, angle, 0);
	}

	//ミサイルと範囲攻撃の寿命管理
	CheckMissilesDuration();
	CheckRangeAttacksDuration();

	Object::Update();
}

void TestBoss::Draw()
{
#ifdef _DEBUG
	ImGui::Begin("boss");
	ImGui::Text("%f", hitPoint);
	ImGui::End();
#endif // _DEBUG

	Object::Draw();
}

void TestBoss::Damage(float arg_value)
{
	hitPoint -= arg_value;	

	if (hitPoint < 0)
	{
		hitPoint = 0;
	}
	
	//HPゲージに反映
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
	//Playerに向けて発射
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
		//フィールドの左右または手前からある程度外れていたら
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
	//Playerの位置を中心として範囲攻撃を生成
	Vector3 playerPos = ActorManager::GetInstance()->GetPlayer()->GetPosition();
	Vector3 emitPos = Vector3(playerPos.x, -5, 0);
	//縦はフィールド全域、横細め
	Vector3 emitScale = Vector3(10, 0.2f, 94);
	float duration = 360.0f;
	BossRangeAttack* rangeAttack = new BossRangeAttack(emitPos, emitScale, duration);
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
