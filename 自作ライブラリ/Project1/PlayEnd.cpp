#include "PlayEnd.h"
#include "Easing.h"

PlayEnd::PlayEnd()
{
	sp_finish = new Sprite();
}

PlayEnd::~PlayEnd()
{
	delete sp_finish;
}

void PlayEnd::Initialize()
{
	isActive = false;
	isFinishEnd = false;
	easingCount_finish = 0;
	positionX_finish = positionX_finish_init;
}

void PlayEnd::Update()
{
	if (!isActive)
		return;

	if (!isFinishEnd)
	{
		TranslationFinishSprite();
	}

	isActive = !isFinishEnd;
}

void PlayEnd::Draw()
{
	if (isActive && !isFinishEnd)
	{
		sp_finish->DrawSprite("GamePlay_UI_Finish", { positionX_finish, 1080 / 2 });
	}
}

void PlayEnd::TranslationFinishSprite()
{
	const float position_end = 960.0f;
	const float easingCount_end = 40.0f;
	const float easingCount_endAdd = 20.0f;//—]‰C

	if (easingCount_finish < easingCount_end)
	{
		positionX_finish = Easing::EaseOutExpo(
			positionX_finish_init, position_end,
			easingCount_end, easingCount_finish);
	}
	else if (easingCount_finish > easingCount_end + easingCount_endAdd)
	{
		easingCount_finish = 0;
		isFinishEnd = true;
	}
	easingCount_finish++;
}
