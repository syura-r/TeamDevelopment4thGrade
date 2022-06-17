#include "Ending.h"

float Ending::panelNum = 0.0f;

Ending::Ending()
{
	next = Title;

	selectState = SelectState::Restart;

	sp_result = new Sprite();
	sp_score = new Sprite();
	numSp_score = new NumberSprite(drawScore);
	sp_panel = new Sprite();
	numSp_panel = new NumberSprite(drawPanelNum);
	sp_select = new Sprite();
	sp_restart = new Sprite();
	sp_title = new Sprite();
}


Ending::~Ending()
{
	delete sp_result;
	delete sp_score;
	delete numSp_score;
	delete sp_panel;
	delete numSp_panel;
	delete sp_select;
	delete sp_restart;
	delete sp_title;
}

void Ending::Initialize()
{
	isEnd = false;

	selectState = SelectState::Restart;
	drawScore = 0.0f;
	score = 50.0f;//�v�Z�����
	isCountEnd_score = false;

	drawPanelNum = 0.0f;
	isCountEnd_panel = false;

	pos_select = pos_restart;
}

void Ending::Update()
{
	SelectMenu();

	//�X�R�A�\�����Z
	if (drawScore < score)
	{
		drawScore += 1;
		//�X�L�b�v
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
		{
			drawScore = score;
		}
	}

	//�擾���\�����Z
	if (drawPanelNum < panelNum)
	{
		drawPanelNum += 1;
		//�X�L�b�v
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
			isCountEnd_score)
		{
			drawPanelNum = panelNum;
		}
	}

	isCountEnd_score = drawScore >= score;
	isCountEnd_panel= drawPanelNum >= panelNum;
}

void Ending::PreDraw()
{
	sp_score->DrawSprite("Result_UI_Gettriangle_text", pos_spScore);//�摜�ς���
	numSp_score->Draw(6, "GamePlay_UI_Number", pos_numScore);

	sp_panel->DrawSprite("Result_UI_Gettriangle_text", pos_spPanel);
	numSp_panel->Draw(6, "GamePlay_UI_Number", pos_numPanel);

	sp_restart->DrawSprite("restart", pos_restart);
	sp_title->DrawSprite("toTitle", pos_title);

	sp_select->DrawSprite("selectInPause", pos_select);
}

void Ending::PostDraw()
{
}

void Ending::SetScore(const float panelNum)
{
	Ending::panelNum = panelNum;
}

void Ending::SelectMenu()
{
	bool isSelectMove = false;//�I����ς�����

	if (Input::TriggerPadLStickLeft())
	{
		selectState = SelectState::Restart;
		isSelectMove = true;
	}
	else if (Input::TriggerPadLStickRight())
	{
		selectState = SelectState::ToTitle;
		isSelectMove = true;
	}

	//�J�[�\���ʒu
	if (isSelectMove)
	{
		switch (selectState)
		{
		case Restart:
			pos_select = pos_restart;
			break;
		case ToTitle:
			pos_select = pos_title;
			break;
		default:
			break;
		}
	}

	//�V�[���؂�ւ�
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
		isCountEnd_score &&
		isCountEnd_panel)
	{
		switch (selectState)
		{
		case Restart:
			next = Play;
			break;
		case ToTitle:
			next = Title;
			break;
		default:
			break;
		}
		ShutDown();
	}
}
