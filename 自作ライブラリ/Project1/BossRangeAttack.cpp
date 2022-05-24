#include "BossRangeAttack.h"
#include "OBJLoader.h"

BossRangeAttack::BossRangeAttack(const Vector3& arg_position, const Vector3& arg_scale, const float arg_durationTime)
	:durationTimer(new Timer(arg_durationTime))
{
	position = arg_position;
	scale = arg_scale;	

	Create(OBJLoader::GetModel("box"));

	Initialize();
}

BossRangeAttack::~BossRangeAttack()
{
	delete durationTimer;
}

void BossRangeAttack::Initialize()
{
	color = { 1,0.1f,0.2f,0.3f };		
	Object::Update();
}

void BossRangeAttack::Update()
{
	durationTimer->Update();
	//”»’è‚ª”­¶‚µ‚½‚çF‚ð”Z‚­
	if (IsActive())
	{
		color.w = 0.6f;
	}
	Object::Update();
}

void BossRangeAttack::Draw()
{
	Object::Draw();
}

bool BossRangeAttack::IsActive()
{
	//§ŒÀŽžŠÔ‚ªŽc‚è”¼•ª‚É‚È‚Á‚½‚çtrue
	return durationTimer->GetRate(TimerPerformance::Down) <= 0.5f;
}

bool BossRangeAttack::IsFinish()
{
	return durationTimer->IsTime();
}
