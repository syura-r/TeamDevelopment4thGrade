#include "Ending.h"
#include "ScoreManager.h"
#include "Audio.h"

Ending::Ending()
{
	next = Title;

	selectState = SelectState::Restart;

	//totalScore = new ResultSet();
	//cutPanel = new ResultSet();
	dropEnemy = new ResultSet();

	sp_select = new Sprite();
	sp_restart = new Sprite();
	sp_title = new Sprite();
}


Ending::~Ending()
{
	//delete totalScore;
	//delete cutPanel;
	delete dropEnemy;

	delete sp_select;
	delete sp_restart;
	delete sp_title;
}

void Ending::Initialize()
{
	isEnd = false;

	selectState = SelectState::Restart;

	ScoreManager* sManager = ScoreManager::GetInstance();
	//totalScore->Initialize(sManager->GetTotalScore());
	//cutPanel->Initialize(sManager->GetCutPanelNum_All());
	dropEnemy->Initialize(sManager->GetFallEnemyNum());

	pos_select = pos_restart;

	Audio::PlayBGM("BGM_Result", 0.1f * Audio::volume_bgm);
}

void Ending::Update()
{
	SelectMenu();

	//各数値の加算
	//totalScore->Update();
	//cutPanel->Update(totalScore->isCountEnd);
	dropEnemy->Update(/*cutPanel->isCountEnd*/);
}

void Ending::PreDraw()
{
	{
		const Vector2 scale_big = { 1.2f, 1.2f };
		const Vector2 scale_mini = { 0.7f, 0.7f };//総スコア以外の項目の大きさ
		//score
		float pos_y = 300.0f;
		//totalScore->Draw("Result_UI_Totalscore_Text", pos_y, scale_big);
		//panel
		//pos_y += 200.0f;
		//cutPanel->Draw("Result_UI_Gettriangle_text", pos_y, scale_mini);
		//enemy
		pos_y += 200.0f;
		dropEnemy->Draw("Result_UI_Drop_text", pos_y, scale_mini);
	}

	//選択項目
	sp_restart->DrawSprite("restart", pos_restart);
	sp_title->DrawSprite("toTitle", pos_title);

	sp_select->DrawSprite("white1x1", pos_select, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,1 });
}

void Ending::PostDraw()
{
}

void Ending::SelectMenu()
{
	bool isSelectMove = false;//選択を変えたか

	if (Input::TriggerPadLStickLeft())
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		selectState = SelectState::Restart;
		isSelectMove = true;
	}
	else if (Input::TriggerPadLStickRight())
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		selectState = SelectState::ToTitle;
		isSelectMove = true;
	}

	//カーソル位置
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

	//シーン切り替え
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
		//totalScore->isCountEnd &&
		//cutPanel->isCountEnd &&
		dropEnemy->isCountEnd)
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
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		Audio::StopSE("BGM_Result");
		ShutDown();
	}
}

Ending::ResultSet::ResultSet()
{
	sp_text = new Sprite();
	numSp_number = new NumberSprite(drawNum);
}

Ending::ResultSet::~ResultSet()
{
	delete sp_text;
	delete numSp_number;
}

void Ending::ResultSet::Initialize(const float arg_num)
{
	num = arg_num;
	drawNum = 0.0f;
	isCountEnd = false;
}

void Ending::ResultSet::Update(const bool skipLook)
{
	//加算して表示
	if (drawNum < num)
	{
		const int sub = num - drawNum;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else if (sub <= 1000)
		{
			addNum = 100;
		}
		else if (sub <= 10000)
		{
			addNum = 1000;
		}
		else if (sub <= 100000)
		{
			addNum = 10000;
		}
		else
		{
			addNum = 100000;
		}

		drawNum += addNum;

		//スキップ
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
			skipLook)
		{
			drawNum = num;
		}
	}

	//加算が終了しているか
	isCountEnd = drawNum >= num;
}

void Ending::ResultSet::Draw(const std::string& arg_texName_text, const float arg_position_y, const Vector2& arg_scale)
{
	//画像の大きさ
	const Vector2 texSize_text = { 640,225 };
	const Vector2 texSize_num = { 47,86 };
	//各数値の桁数
	const int digit = 6;
	//基準X座標
	const float position_text_x = 700.0f;
	const float position_number_x = position_text_x + (texSize_text.x / 2) + (texSize_num.x / 2 * (digit * 2 - 1));//文字のすぐ後ろの位置

	sp_text->DrawSprite(arg_texName_text, { position_text_x, arg_position_y }, 0.0f, arg_scale);
	numSp_number->Draw(digit, "GamePlay_UI_Number", { position_number_x, arg_position_y }, arg_scale);
}
