#include "NormalWaveMeasurer.h"
#include "ActorManager.h"
#include "Player.h"

NormalWaveMeasurer::NormalWaveMeasurer()
{
	timer = new Timer(900);
	nowTimeSprite = new Sprite();
	baseSprite = new Sprite();
	nowSecSprite = new NumberSprite(nowSec);
}

NormalWaveMeasurer::~NormalWaveMeasurer()
{
	delete timer;
	delete nowTimeSprite;
	delete baseSprite;
	delete nowSecSprite;
}

void NormalWaveMeasurer::Initialize()
{
	timer->Initialize();
}

void NormalWaveMeasurer::Update()
{
	timer->Update();

	nowSec = timer->GetRealTime(TimerPerformance::Down) + 1;
}

void NormalWaveMeasurer::Draw()
{
	XMFLOAT2 basepos = XMFLOAT2(100, 800);
	
	if (!ActorManager::GetInstance()->GetPlayer()->IsInFever())
	{
		int timeDigit = nowSec / 10 + 1;
		if (timeDigit > 2)
		{
			timeDigit = 2;
		}

		float nowTimePosX;
		if (timeDigit == 2)
		{
			nowTimePosX = 130;
		}
		else
		{
			nowTimePosX = 150;
		}
		nowTimeSprite->DrawSprite("white1x1", basepos, 0.0f, XMFLOAT2(100, 700 * timer->GetRate(TimerPerformance::Down)), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
		nowSecSprite->Draw(timeDigit, "number", Vector2(nowTimePosX, 50), Vector2(0.7f, 0.70f), Vector4(1, 1, 1, 1), Vector2(0, 0));
	}

	baseSprite->DrawSprite("white1x1", basepos, 0.0f, XMFLOAT2(100, 700), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f), XMFLOAT2(0.0f, 1.0f));
}

bool NormalWaveMeasurer::IsTime()
{
	return timer->IsTime();
}

void NormalWaveMeasurer::Reset()
{
	int quota;
	if (ActorManager::GetInstance()->GetPlayer())
	{
		quota = ActorManager::GetInstance()->GetPlayer()->GetFeverQuota();
	}
	else
	{
		quota = 3;
	}
	float time;
	switch (quota)
	{
	case 3:
		time = 15;
		break;
	case 4:
		time = 23;
		break;
	case 5:
		time = 30;
		break;
	case 6:
		time = 40;
		break;
	default:
		time = 90;
		break;
	}

	timer->SetLimit(time * 60);
	timer->Reset();
}

void NormalWaveMeasurer::Reset(const float arg_sec)
{
	timer->SetLimit(arg_sec * 60);
	timer->Reset();
}
