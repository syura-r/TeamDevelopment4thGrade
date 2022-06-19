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

	//�I��
	bool isSelectMove = false;//�I����ς�����
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

	//�J�[�\���ʒu
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

	//����
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");
		switch (selectState)
		{
		case ToGame:
			//�Q�[���ɂ��ǂ�
			useFlag = false;
			break;
		case Restart:
			//��蒼��
			useFlag = false;
			flag_restart = true;
			break;
		case ToTitle:
			//�^�C�g���ɂ��ǂ�
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
