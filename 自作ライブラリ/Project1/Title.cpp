#include "Title.h"

#include "DebugCamera.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "Audio.h"

Title::Title()
{
	next = Play;

	titleLogo = new Sprite();
	titleStart = new Sprite();
}


Title::~Title()
{
	delete titleLogo;
	delete titleStart;
}

void Title::Initialize()
{
	isEnd = false;

	Audio::PlayWave("BGM_Title", 0.1f, true);
	Audio::PlayWave("BGM_Title", 0.1f, true);
}

void Title::Update()
{
	//ƒV[ƒ“Ø‚è‘Ö‚¦
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");
		Audio::StopWave("BGM_Title");
		ShutDown();
	}

//#ifdef _DEBUG
	if (Input::TriggerKey(DIK_SPACE))
	{
		Audio::PlayWave("SE_Decision");
		Audio::StopWave("BGM_Title");
		ShutDown();
	}
//#endif

	if (Input::TriggerKey(DIK_1))
	{
		Audio::Stop("BGM_Title");
	}
	if (Input::TriggerKey(DIK_2))
	{
		Audio::Play("BGM_Title");
	}
}

void Title::PreDraw()
{
	const Vector2 windowCenterPosition = { 960,500 };
	const float startcharPlusPositionY = 280;

	const float scale = 2.0f;
	titleLogo->DrawSprite("titlelogo", windowCenterPosition, 0.0f, { scale, scale });
	titleStart->DrawSprite("titlestart", { windowCenterPosition.x, windowCenterPosition.y + startcharPlusPositionY }, 0.0f, {1.5f, 1.5f});
}

void Title::PostDraw()
{
}