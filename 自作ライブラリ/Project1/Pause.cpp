#include "Pause.h"
#include"Input.h"
#include "Audio.h"
#include "Easing.h"

float Pause::positions_X[Pause::positionStepMax] = { -100.0f, 160.0f, 300.0f, 600.0f };
bool Pause::fadeFlag = false;

Pause::Pause()
{
	sp_back = new Sprite();
	sp_base = new Sprite();
	toGame = new SelectSprite();
	restart = new SelectSprite();
	toTitle = new SelectSprite();
	sound = new SelectSprite();
	bgm = new SelectSprite();
	bar_bgm = new Sprite();
	circle_bgm = new Sprite();
	se = new SelectSprite();
	bar_se = new Sprite();
	circle_se = new Sprite();
	config_sp = new Sprite();
}

Pause::~Pause()
{
	delete sp_back;
	delete sp_base;
	delete toGame;
	delete restart;
	delete toTitle;
	delete sound;
	delete bgm;
	delete bar_bgm;
	delete circle_bgm;
	delete se;
	delete bar_se;
	delete circle_se;
	delete config_sp;
}

void Pause::Initialize()
{
	activeFlag = false;
	fadeFlag = false;

	selectState = ToGame;
	selectState_sound = BGM;

	float pos_y_standard = 1080.0f / (selectMax + 2);
	toGame->Initialize("toGame", pos_y_standard);
	flag_toGame = false;
	restart->Initialize("restart", pos_y_standard * 2);
	flag_restart = false;
	toTitle->Initialize("toTitle", pos_y_standard * 3);
	flag_toTitle = false;
	sound->Initialize("sound", pos_y_standard * 4);
	flag_sound = false;
	bgm->Initialize("BGM", pos_y_standard * 4);
	barPositionLeft_bgm = { positions_X[3] + 150.0f,bgm->pos.y };
	circlePosition_bgm = barPositionLeft_bgm;
	se->Initialize("SE", pos_y_standard * 5);
	barPositionLeft_se = { positions_X[3] + 150.0f,se->pos.y };
	circlePosition_se = barPositionLeft_se;

	pos_base = toGame->pos;
	alpha_base = 1.0f;
	isUP_alphaChange = false;
}

void Pause::Update()
{
	//�|�[�Y�̃I���I�t
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START) || Input::TriggerKey(DIK_ESCAPE))
	{
		activeFlag = !activeFlag;

		if (activeFlag)
		{
			Audio::AllPauseSE();
		}
		else
		{
			Audio::AllResumeSE();
		}

		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		if (!activeFlag)
		{
			fadeFlag = true;

			toGame->step = 0;
			restart->step = 0;
			toTitle->step = 0;
			sound->step = 0;
			bgm->step = 0;
			se->step = 0;

			toGame->PreMoveSetting();
			restart->PreMoveSetting();
			toTitle->PreMoveSetting();
			sound->PreMoveSetting();
			bgm->PreMoveSetting();
			se->PreMoveSetting();
		}
	}

	if (!activeFlag && !fadeFlag)
	{
		Initialize();
		return;
	}

	if (!fadeFlag)
	{
		//�I��
		if (flag_sound)
		{
			SelectSub_Sound();
		}
		else
		{
			Select();
		}
		//�_��
		const float speed_alphaChange = 0.02f;//���x
		const float min_alphaChange = 0.3f;//����
		const float max_alphaChange = 1.0f;//���
		if (isUP_alphaChange)//�s������
		{
			alpha_base += speed_alphaChange;
			if (alpha_base >= max_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		else//������
		{
			alpha_base -= speed_alphaChange;
			if (alpha_base <= min_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		//����
		Decision();
	}

	//base�̍��W�ύX
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
		//�T�u���j���[
		if (flag_sound)
		{
			switch (selectState_sound)
			{
			case BGM:
				pos_base = bgm->pos;
				break;
			case SE:
				pos_base = se->pos;
				break;
			default:
				break;
			}
		}
		break;

	default:
		break;
	}

	toGame->Update();
	restart->Update();
	toTitle->Update();
	sound->Update();
	bgm->Update();
	se->Update();
}

void Pause::Draw()
{
	if (!activeFlag && !fadeFlag)
		return;

	toGame->Draw();
	restart->Draw();
	toTitle->Draw();
	sound->Draw();
	if (flag_sound)
	{
		bgm->Draw();
		se->Draw();

		if (!fadeFlag)
		{
			const Vector2 scale_circle = { 0.7f, 0.7f };
			bar_bgm->DrawSprite("white1x1", barPositionLeft_bgm, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
			circle_bgm->DrawSprite("circle", circlePosition_bgm, 0.0f, scale_circle);
			bar_se->DrawSprite("white1x1", barPositionLeft_se, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
			circle_se->DrawSprite("circle", circlePosition_se, 0.0f, scale_circle);
		}

	}

	config_sp->DrawSprite("GamePlay_UI_Operation", { 960,400 }, 0.0f, { 0.9f, 0.9f });

	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	sp_base->DrawSprite("white1x1", pos_base, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Pause::Select()
{
	bool isSelectMove = false;//�I����ς�����
	int select = selectState;
	if ((Input::TriggerPadLStickUp() || Input::TriggerKey(DIK_W)) && selectState > 0)
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		isSelectMove = true;
		select--;
	}
	else if ((Input::TriggerPadLStickDown() || Input::TriggerKey(DIK_S)) && selectState < selectMax - 1)
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		isSelectMove = true;
		select++;
	}
	selectState = (SelectState)select;

	//�J�[�\���ʒu
	if (isSelectMove)
	{
		//��U�S����I�𖳂��̐ݒ��
		toGame->step = 1;
		restart->step = 1;
		toTitle->step = 1;
		sound->step = 1;

		toGame->PreMoveSetting();
		restart->PreMoveSetting();
		toTitle->PreMoveSetting();
		sound->PreMoveSetting();
		bgm->PreMoveSetting();
		se->PreMoveSetting();

		//�����x���Z�b�g
		alpha_base = 1.0f;
		isUP_alphaChange = false;
	}

	//�ʒu�����炷
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
		bgm->step = 3;
		se->step = 3;
		break;

	default:
		break;
	}
}

void Pause::SelectSub_Sound()
{
	bool isSelectMove = false;//�I����ς�����
	int select = selectState_sound;
	if ((Input::TriggerPadLStickUp() || Input::TriggerKey(DIK_W)) && selectState_sound > 0)
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		isSelectMove = true;
		select--;
	}
	else if ((Input::TriggerPadLStickDown() || Input::TriggerKey(DIK_S)) && selectState_sound < selectMax_sound - 1)
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		isSelectMove = true;
		select++;
	}
	selectState_sound = (SelectState_Sound)select;

	//�e�ݒ胂�[�h
	const float volumeMax = 2.0f;
	const float volumeOneScale= 0.1f;
	float stock_volume_bgm = Audio::volume_bgm;
	float stock_volume_se = Audio::volume_se;
	switch (selectState_sound)
	{
	case BGM:
		//���ʕύX
		if ((Input::CheckPadLStickLeft() || Input::DownKey(DIK_A)) && stock_volume_bgm > 0.0f)
		{
			stock_volume_bgm -= volumeOneScale;
		}
		else if ((Input::CheckPadLStickRight() || Input::DownKey(DIK_D)) && volumeMax > stock_volume_bgm)
		{
			stock_volume_bgm += volumeOneScale;
		}
		break;

	case SE:
		//���ʕύX
		if ((Input::CheckPadLStickLeft() || Input::DownKey(DIK_A)) && stock_volume_se > 0.0f)
		{
			stock_volume_se -= volumeOneScale;
		}
		else if ((Input::CheckPadLStickRight() || Input::DownKey(DIK_D)) && volumeMax > stock_volume_se)
		{
			stock_volume_se += volumeOneScale;
		}
		break;

	default:
		break;
	}
	//�ۂ̈ʒu�ύX
	circlePosition_bgm.x = barPositionLeft_bgm.x + (stock_volume_bgm * (bar_scale.x / volumeMax));
	circlePosition_se.x = barPositionLeft_se.x + (stock_volume_se * (bar_scale.x / volumeMax));

	//audio�̕ϐ��ɔ��f
	Audio::volume_bgm = stock_volume_bgm;
	Audio::volume_se = stock_volume_se;
}

void Pause::Decision()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
	{
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);

		//�ݒ�����
		activeFlag = false;

		switch (selectState)
		{
		case ToGame:
			//�Q�[���ɂ��ǂ�
			flag_toGame = true;
			break;
		case Restart:
			//��蒼��
			flag_restart = true;
			break;
		case ToTitle:
			//�^�C�g���ɂ��ǂ�
			flag_toTitle = true;
			break;
		case Sound:
			//���ʐݒ�
			flag_sound = !flag_sound;
			//�ݒ����Ȃ�
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
	step++;//���W�̐ݒ��ɕύX����
	easeTime = 0;
}

void Pause::SelectSprite::Update()
{
	//�C�[�W���O�p�̃^�C�}�[�@�\
	const int timeLimit = 5;
	if (easeTime >= timeLimit)
	{
		easeTime = timeLimit;
		fadeFlag = false;
	}
	else
	{
		easeTime++;

		//�ړ�
		pos.x = Easing::EaseOutQuad(prevPos.x, positions_X[step], timeLimit, easeTime);
	}
}

void Pause::SelectSprite::Draw()
{
	sprite->DrawSprite(texName, pos);
}

void Pause::SelectSprite::PreMoveSetting()
{
	//�ړ��O�̍��W�ێ�
	prevPos = pos;

	//�ړ��J�n
	easeTime = 0;
}
