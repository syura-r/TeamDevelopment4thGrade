#include "Pause.h"
#include"Input.h"
#include "Audio.h"

Pause::Pause()
{
	sp_base = new Sprite();
	sp_toGame = new Sprite();
	sp_toTitle = new Sprite();
	sp_restart = new Sprite();
}

Pause::~Pause()
{
	delete sp_base;
	delete sp_toGame;
	delete sp_toTitle;
	delete sp_restart;
}

void Pause::Initialize()
{
	useFlag = false;
	selectState = ToGame;

	pos_toGame = {960, 340};
	pos_base = pos_toGame;

	pos_restart = {960, 540};
	flag_restart = false;

	pos_toTitle = {960, 740};
	flag_toTitle = false;
}

void Pause::Update()
{
	if (!useFlag)
		return;

	//選択
	bool isSelectMove = false;//選択を変えたか
	int select = selectState;
	if (Input::TriggerPadLStickUp() && selectState > 0)
	{
		Audio::PlayWave("SE_Select");
		isSelectMove = true;
		select--;
	}
	else if (Input::TriggerPadLStickDown() && selectState < selectMax - 1)
	{
		Audio::PlayWave("SE_Select");
		isSelectMove = true;
		select++;
	}
	selectState = (SelectState)select;

	//カーソル位置
	if (isSelectMove)
	{
		switch (selectState)
		{
		case ToGame:
			pos_base = pos_toGame;
			break;
		case Restart:
			pos_base = pos_restart;
			break;
		case ToTitle:
			pos_base = pos_toTitle;
			break;
		default:
			break;
		}
	}

	//決定
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");
		switch (selectState)
		{
		case ToGame:
			//ゲームにもどる
			useFlag = false;
			break;
		case Restart:
			//やり直し
			useFlag = false;
			flag_restart = true;
			break;
		case ToTitle:
			//タイトルにもどる
			useFlag = false;
			flag_toTitle = true;
			break;
		default:
			break;
		}
	}
}

void Pause::Draw()
{
	if (!useFlag)
		return;

	sp_toGame->DrawSprite("toGame", pos_toGame);
	sp_restart->DrawSprite("restart", pos_restart);
	sp_toTitle->DrawSprite("toTitle", pos_toTitle);

	sp_base->DrawSprite("selectInPause", pos_base);
}
