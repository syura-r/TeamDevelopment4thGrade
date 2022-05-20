#include "NormalWaveMeasurer.h"

NormalWaveMeasurer::NormalWaveMeasurer()
{
	timer = new Timer(900);
	nowTimeSprite = new Sprite();
	baseSprite = new Sprite();
}

NormalWaveMeasurer::~NormalWaveMeasurer()
{
	delete timer;
	delete nowTimeSprite;
	delete baseSprite;
}

void NormalWaveMeasurer::Initialize()
{
	timer->Initialize();
}

void NormalWaveMeasurer::Update()
{
	timer->Update();

	if (timer->IsTime())
	{
		timer->Reset();
	}
}

void NormalWaveMeasurer::Draw()
{
	XMFLOAT2 basepos = XMFLOAT2(100, 800);
	nowTimeSprite->DrawSprite("white1x1", basepos, 0.0f, XMFLOAT2(100, 700 * timer->GetRate(TimerPerformance::Down)), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	baseSprite->DrawSprite("white1x1", basepos, 0.0f, XMFLOAT2(100, 700), XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f), XMFLOAT2(0.0f, 1.0f));
}
