#include "Ending.h"
#include "Audio.h"
#include "Input.h"
#include "FBXManager.h"
#include "OBJLoader.h"

int Ending::score_player = 0;
int Ending::score_enemy_red = 0;
int Ending::score_enemy_green = 0;

Ending::Ending()
{
	next = Title;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());

	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	selectState = SelectState::Restart;

	set[0] = new ResultSet(ActorTag::PLAYER);
	set[1] = new ResultSet(ActorTag::ENEMY_RED);
	set[2] = new ResultSet(ActorTag::ENEMY_GREEN);


	sp_select = new Sprite();
	sp_title = new Sprite();
	sp_restart = new Sprite();

	stadium = new Stadium();
}


Ending::~Ending()
{
	for (int i = 0; i < enemyCount + 1; i++)
	{
		delete set[i];
	}

	delete sp_select;
	delete sp_title;
	delete sp_restart;

	delete stadium;
}

void Ending::Initialize()
{
	isEnd = false;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	selectState = SelectState::ToTitle;

	for (int i = 0; i < enemyCount + 1; i++)
	{
		int score = 0;
		if (i == 0)
			score = score_player;
		else if (i == 1)
			score = score_enemy_red;
		else
			score = score_enemy_green;
		set[i]->Initialize(score, positions_3d[i], positions_2d[i]);
	}
	topScore = 0;
	TopSearch();
	GaugeSize();
	isSkipOrFinish = false;

	pos_select = pos_title;
	scale_select = scaleBig_select;
	alpha_select = 1.0f;
	isUP_alphaChange = false;

	scale_title = scaleBig_select;
	alpha_title = 1.0f;
	scale_restart = scaleSmall_select;
	alpha_restart = 1.0f;

	Audio::PlayBGM("BGM_Result", 0.1f * Audio::volume_bgm);

	stadium->Initialize();
	stadium->SetPosition({ 0.0f,-40.0f,0.0f });
}

void Ending::Update()
{
	SelectMenu();

	FlashMenu();

	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->Update(topScore);
	}
	stadium->Update();

	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,0.0f,0.2f,1 });
	lightGroup->Update();
}

void Ending::PreDraw()
{
	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->Draw();
	}

	//選択項目
	sp_title->DrawSprite("toTitle", pos_title, 0.0f, scale_title, { 1,1,1,alpha_title });
	sp_restart->DrawSprite("restart", pos_restart, 0.0f, scale_restart, { 1,1,1,alpha_restart });

	stadium->Draw();


	Vector2 size_select = { 256.0f, 64.0f };
	sp_select->DrawSprite("white1x1", pos_select, 0.0f, size_select * scale_select, { 0.3f,0.3f,0.3f,alpha_select }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");

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
		selectState = SelectState::ToTitle;
		isSelectMove = true;
	}
	else if (Input::TriggerPadLStickRight())
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		selectState = SelectState::Restart;
		isSelectMove = true;
	}

	//カーソル位置
	if (isSelectMove)
	{
		switch (selectState)
		{
		case ToTitle:
			pos_select = pos_title;
			break;
		case Restart:
			pos_select = pos_restart;
			break;
		default:
			break;
		}
		//透明度リセット
		alpha_select = 1.0f;
		alpha_title = 1.0f;
		alpha_restart = 1.0f;
		isUP_alphaChange = false;
	}

	//拡縮
	const float speed_scale = 0.1f;
	switch (selectState)
	{
	case ToTitle:
		if (scale_title.x < scaleBig_select.x)
		{
			scale_title.x += speed_scale;
			scale_title.y += speed_scale;
		}
		if (scale_restart.x > scaleSmall_select.x)
		{
			scale_restart.x -= speed_scale;
			scale_restart.y -= speed_scale;
		}
		break;
	case Restart:
		if (scale_restart.x < scaleBig_select.x)
		{
			scale_restart.x += speed_scale;
			scale_restart.y += speed_scale;
		}
		if (scale_title.x > scaleSmall_select.x)
		{
			scale_title.x -= speed_scale;
			scale_title.y -= speed_scale;
		}
		break;
	default:
		break;
	}

	//シーン切り替え
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
		isSkipOrFinish)
	{
		switch (selectState)
		{
		case ToTitle:
			next = Title;
			break;
		case Restart:
			next = Play;
			break;
		default:
			break;
		}
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		Audio::StopBGM("BGM_Result");
		ShutDown();
	}

	//スキップ
	MotionSkip();
}

void Ending::FlashMenu()
{
	const float speed_alphaChange = 0.02f;//速度
	const float min_alphaChange = 0.3f;//下限
	const float max_alphaChange = 1.0f;//上限
	if (isUP_alphaChange)//不透明に
	{
		alpha_select += speed_alphaChange;
		if (alpha_select >= max_alphaChange)
		{
			isUP_alphaChange = !isUP_alphaChange;
		}
	}
	else//透明に
	{
		alpha_select -= speed_alphaChange;
		if (alpha_select <= min_alphaChange)
		{
			isUP_alphaChange = !isUP_alphaChange;
		}
	}

	//反映
	//switch (selectState)
	//{
	//case ToTitle:
	//	alpha_title = alpha_select;
	//	break;
	//case Restart:
	//	alpha_restart = alpha_select;
	//	break;
	//default:
	//	break;
	//}
}

void Ending::TopSearch()
{
	bool isTops[enemyCount + 1] = { false,false,false };
	for (int i = 0; i < enemyCount+1; i++)
	{
		//0以下はスルー
		if (set[i]->score <= 0)
			continue;

		//記録更新
		if (topScore < set[i]->score)
		{
			topScore = set[i]->score;
			for (int j = 0; j < enemyCount+1; j++)
			{
				isTops[j] = false;
			}
			isTops[i] = true;
		}
		//同率
		else if (topScore == set[i]->score)
		{
			topScore = set[i]->score;
			isTops[i] = true;
		}
	}

	//反映
	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->isTop = isTops[i];
	}
}

void Ending::GaugeSize()
{
	//一位を基準（1.0f）にする
	float topCount = 0.0f;
	for (int i = 0; i < enemyCount+1; i++)
	{
		if (set[i]->isTop)
		{
			set[i]->scaleY_gauge = 1.0f;
			topCount = set[i]->score;
		}
	}

	//
	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->scaleY_gauge = set[i]->score / topCount;
		//全員0の場合
		if (topCount <= 0.0f)
			set[i]->scaleY_gauge = 0.0f;
	}
}

void Ending::MotionSkip()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) &&
		!isSkipOrFinish)
	{
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		isSkipOrFinish = true;
	}

	if (isSkipOrFinish)
	{
		for (int i = 0; i < enemyCount + 1; i++)
		{
			set[i]->score_draw = set[i]->score;
			set[i]->scaleY_gauge_draw = set[i]->scaleY_gauge;
		}
	}
	else
	{
		//全てのゲージと数値の動きが終わったら真
		isSkipOrFinish = true;
		for (int i = 0; i < enemyCount + 1; i++)
		{
			isSkipOrFinish =
				set[i]->score_draw >= set[i]->score &&
				set[i]->scaleY_gauge_draw >= set[i]->scaleY_gauge &&
				isSkipOrFinish;
		}
	}
}

Ending::ResultSet::ResultSet(const ActorTag& arg_tag)
	:tag(arg_tag)
{
	std::string modelName;
	switch (tag)
	{
	case ActorTag::PLAYER:
		modelName = "GamePlay_Player";
		break;
	case ActorTag::ENEMY_RED:
		modelName = "GamePlay_Enemy";
		break;
	case ActorTag::ENEMY_GREEN:
		modelName = "GamePlay_Enemy2";
		break;
	default:
		break;
	}

	object = Object3D::Create(FBXManager::GetModel(modelName), position, scale, rotation, color);
	obj_crown = Object3D::Create(OBJLoader::GetModel("Crown"), pos_crown, scale_crown, rotation_crown, color_crown);
	numberSprite = new NumberSprite(score_draw);
	sp_gauge = new Sprite();
}

Ending::ResultSet::~ResultSet()
{
	delete object;
	delete obj_crown;
	delete numberSprite;
	delete sp_gauge;
}

void Ending::ResultSet::Initialize(const int arg_score, const float arg_positionX_3d, const float arg_positionX_2d)
{
	score = (float)arg_score;
	score_draw = 0.0f;
	position.x = arg_positionX_3d;
	pos_crown.x = arg_positionX_3d;
	positionX_2d = arg_positionX_2d;
	scaleY_gauge = 0.0f;
	scaleY_gauge_draw = 0.0f;
	isTop = false;
	drawCrown = false;
}

void Ending::ResultSet::Update(const int arg_topScore)
{
	//ゲージを伸ばす
	const bool gaugeExtend = scaleY_gauge_draw < scaleY_gauge;
	if (gaugeExtend)
	{
		scaleY_gauge_draw += 1.0f / 120.0f;
	}

	//数値を増やす
	const bool scoreAdd = score_draw < score;
	if (scoreAdd)
	{
		score_draw += arg_topScore / 120.0f;
	}

	//王冠の表示
	drawCrown = !gaugeExtend && !scoreAdd;

	object->Update();
	obj_crown->Update();
}

void Ending::ResultSet::Draw()
{
	PipelineState::SetPipeline("FBX");
	object->Draw(true);

	if (isTop && drawCrown)
	{
		PipelineState::SetPipeline("BasicObj");
		obj_crown->Draw();
	}

	PipelineState::SetPipeline("Sprite");

	std::string nuwNum = std::to_string((int)score_draw);
	int digit = nuwNum.size();//0で埋めないため
	numberSprite->Draw(digit, "GamePlay_UI_Number", { positionX_2d, 200.0f });

	std::string texName;
	switch (tag)
	{
	case ActorTag::PLAYER:
		texName = "Result_Player_Gauge";
		break;
	case ActorTag::ENEMY_RED:
		texName = "Result_Enemy_Gauge";
		break;
	case ActorTag::ENEMY_GREEN:
		texName = "Result_Enemy2_Gauge";
		break;
	default:
		break;
	}

	sp_gauge->DrawSprite(texName, { positionX_2d, 770.0f }, 0.0f, { 1.0f, scaleY_gauge_draw }, { 1,1,1,1 }, { 0.5f, 1.0f });
}
