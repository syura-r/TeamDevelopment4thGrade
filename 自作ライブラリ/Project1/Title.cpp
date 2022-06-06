#include "Title.h"

#include "DebugCamera.h"
#include "Sprite.h"
#include "TextureResource.h"

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
}

void Title::Update()
{
	//ƒV[ƒ“Ø‚è‘Ö‚¦
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		ShutDown();
	}
}

void Title::PreDraw()
{
	const Vector2 windowCenterPosition = { 960,540 };
	const float startcharPlusPositionY = 250;

	titleLogo->DrawSprite("titlelogo", windowCenterPosition);
	titleStart->DrawSprite("titlestart", { windowCenterPosition.x, windowCenterPosition.y + startcharPlusPositionY });
}

void Title::PostDraw()
{
}