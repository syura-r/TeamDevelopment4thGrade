#include "Pause.h"
#include"Input.h"
#include "Audio.h"
#include "Easing.h"

float Pause::positions_X[Pause::positionStepMax] = { -100.0f, 160.0f, 300.0f };
bool Pause::fadeFlag = false;

Pause::Pause()
{
	sp_back = new Sprite();
	sp_base = new Sprite();
	toGame = new SelectSprite();
	restart = new SelectSprite();
	toTitle = new SelectSprite();
	sound = new SelectSprite();
}

Pause::~Pause()
{
	delete sp_back;
	delete sp_base;
	delete toGame;
	delete restart;
	delete toTitle;
	delete sound;
}

void Pause::Initialize()
{
	activeFlag = false;
	fadeFlag = false;

	selectState = ToGame;

	float pos_y_standard = 1080.0f / (selectMax + 1);
	toGame->Initialize("toGame", pos_y_standard);
	flag_toGame = false;
	restart->Initialize("restart", pos_y_standard * 2);
	flag_restart = false;
	toTitle->Initialize("toTitle", pos_y_standard * 3);
	flag_toTitle = false;
	sound->Initialize("sound", pos_y_standard * 4);
	flag_sound = false;

	pos_base = toGame->pos;
}

void Pause::Update()
{
	//ポーズのオンオフ
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		activeFlag = !activeFlag;
		if (!activeFlag)
		{
			fadeFlag = true;

			toGame->step = 0;
			restart->step = 0;
			toTitle->step = 0;
			sound->step = 0;

			toGame->PreMoveSetting();
			restart->PreMoveSetting();
			toTitle->PreMoveSetting();
			sound->PreMoveSetting();
		}
	}

	if (!activeFlag && !fadeFlag)
	{
		Initialize();
		return;
	}

	if (!fadeFlag)
	{
		//選択
		Select();
		//決定
		Decision();
	}

	//baseの座標変更
	switch (selectState)
	{
	case ToGame:
		pos_base = toGame->pos;
		break;

	case Restart:
		pos_base = restart->pos;
		break;

	case ToTitle:
		pos_base = toTitle->pos;
		break;

	case Sound:
		pos_base = sound->pos;
		break;

	default:
		break;
	}

	toGame->Update();
	restart->Update();
	toTitle->Update();
	sound->Update();
}

void Pause::Draw()
{
	if (!activeFlag && !fadeFlag)
		return;

	toGame->Draw();
	restart->Draw();
	toTitle->Draw();
	sound->Draw();

	sp_base->DrawSprite("selectInPause", pos_base);

	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");
}

void Pause::Select()
{
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
		//一旦全部を選択無しの設定に
		toGame->step = 1;
		restart->step = 1;
		toTitle->step = 1;
		sound->step = 1;

		toGame->PreMoveSetting();
		restart->PreMoveSetting();
		toTitle->PreMoveSetting();
		sound->PreMoveSetting();
	}

	//位置をずらす
	switch (selectState)
	{
	case ToGame:
		toGame->step = 2;
		break;

	case Restart:
		restart->step = 2;
		break;

	case ToTitle:
		toTitle->step = 2;
		break;

	case Sound:
		sound->step = 2;
		break;

	default:
		break;
	}
}

void Pause::Decision()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");

		//設定を閉じる
		activeFlag = false;

		switch (selectState)
		{
		case ToGame:
			//ゲームにもどる
			flag_toGame = true;
			break;
		case Restart:
			//やり直し
			flag_restart = true;
			break;
		case ToTitle:
			//タイトルにもどる
			flag_toTitle = true;
			break;
		case Sound:
			//音量設定
			flag_sound = true;
			//設定を閉じない
			activeFlag = true;
			break;
		default:
			break;
		}
	}
}

Pause::SelectSprite::SelectSprite()
{
	sprite = new Sprite();
}

Pause::SelectSprite::~SelectSprite()
{
	delete sprite;
}

void Pause::SelectSprite::Initialize(const std::string& texName, const float posY)
{
	this->texName = texName;
	step = 0;
	pos = { positions_X[step], posY };
	prevPos = pos;
	step++;//座標の設定後に変更する
	easeTime = 0;
}

void Pause::SelectSprite::Update()
{
	//イージング用のタイマー機能
	const int timeLimit = 5;
	if (easeTime >= timeLimit)
	{
		easeTime = timeLimit;
		fadeFlag = false;
	}
	else
	{
		easeTime++;

		//移動
		pos.x = Easing::EaseOutQuad(prevPos.x, positions_X[step], timeLimit, easeTime);
	}
}

void Pause::SelectSprite::Draw()
{
	sprite->DrawSprite(texName, pos);
}

void Pause::SelectSprite::PreMoveSetting()
{
	//移動前の座標保持
	prevPos = pos;

	//移動開始
	easeTime = 0;
}
