#include "Ending.h"
#include "Audio.h"
#include "Input.h"
#include "OBJLoader.h"

int Ending::score_player = 0;
int Ending::score_enemy_red = 0;
int Ending::score_enemy_green = 0;

Ending::Ending()
{
	next = Title;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());

	lightGroup.reset(LightGroup::Create());
	Object3D::SetLightGroup(lightGroup.get());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	selectState = SelectState::ToTitle;

	sp_select = new Sprite();
	sp_title = new Sprite();
	sp_restart = new Sprite();

	const std::string modelName[actorsCount] = {
		"GamePlay_Player",
		"GamePlay_Enemy",
		"GamePlay_Enemy2"
	};
	for (int i = 0; i < actorsCount; i++)
	{
		actors[i] = new EndingActor(modelName[i]);
	}
	basePanel_object = Object3D::Create(OBJLoader::GetModel("fieldPiece"), basePanel_position, basePanel_scale, basePanel_rotation, basePanel_color);
	stadium = new Stadium();
}


Ending::~Ending()
{
	delete sp_select;
	delete sp_title;
	delete sp_restart;

	for (int i = 0; i < actorsCount; i++)
	{
		delete actors[i];
	}
	delete basePanel_object;
	delete stadium;
}

void Ending::Initialize()
{
	isEnd = false;

	camera->Initialize();
	camera.get()->SetDistance(30);
	camera.get()->SetTheta(0.5f);
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	selectState = SelectState::ToTitle;

	pos_select = pos_title;
	scale_select = scaleBig_select;
	alpha_select = 1.0f;
	isUP_alphaChange = false;

	scale_title = scaleBig_select;
	alpha_title = 1.0f;
	scale_restart = scaleSmall_select;
	alpha_restart = 1.0f;

	RankingSearch();
	actors[0]->Initialize(score_player, ranking[0]);
	actors[1]->Initialize(score_enemy_red, ranking[1]);
	actors[2]->Initialize(score_enemy_green, ranking[2]);

	stadium->Initialize();
	stadium->SetPosition({ 0.0f,-40.0f,0.0f });

	Audio::PlayBGM("BGM_Result", 0.1f * Audio::volume_bgm);
}

void Ending::Update()
{
	SelectMenu();
	FlashMenu();

	for (int i = 0; i < actorsCount; i++)
	{
		actors[i]->Update();

		//1位をカメラで追う
		if (actors[i]->GetRanking() == 1)
			camera.get()->SetTarget(actors[i]->GetPosition());
		//数値表示
		if (actors[0]->GetIsAddPanelEnd() &&
			actors[1]->GetIsAddPanelEnd() &&
			actors[2]->GetIsAddPanelEnd())
		{
			actors[i]->SetIsNumberRoll(false);
		}
	}

	basePanel_object->Update();
	stadium->Update();

	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,-1.0f,0.2f,1 });
	lightGroup->Update();
}

void Ending::PreDraw()
{
	//選択項目
	sp_title->DrawSprite("toTitle", pos_title, 0.0f, scale_title, { 1,1,1,alpha_title });
	sp_restart->DrawSprite("restart", pos_restart, 0.0f, scale_restart, { 1,1,1,alpha_restart });

	for (int i = 0; i < actorsCount; i++)
	{
		actors[i]->Draw();
	}
	stadium->Draw();
	basePanel_object->Draw();

	Vector2 size_select = { 256.0f, 64.0f };
	sp_select->DrawSprite("white1x1", pos_select, 0.0f, size_select * scale_select, { 0.3f,0.3f,0.3f,alpha_select }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");

}

void Ending::PostDraw()
{
}

void Ending::SelectMenu()
{
	bool isSelectMove = false;//選択を変えたか

	if (Input::TriggerPadLStickLeft() || Input::TriggerKey(DIK_A))
	{
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		selectState = SelectState::ToTitle;
		isSelectMove = true;
	}
	else if (Input::TriggerPadLStickRight() || Input::TriggerKey(DIK_D))
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
	bool isEnd = actors[0]->GetIsAddPanelEnd() &&
		actors[1]->GetIsAddPanelEnd() &&
		actors[2]->GetIsAddPanelEnd();
	if ((Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)) && isEnd)
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

void Ending::RankingSearch()
{
	enum ActorTag
	{
		PLAYER,
		ENEMY_RED,
		ENEMY_GREEN
	};
	struct ScoreSet
	{
		ActorTag tag;
		int score;
	};
	ScoreSet sets[actorsCount] =
	{
		{ActorTag::PLAYER, score_player},
		{ActorTag::ENEMY_RED, score_enemy_red},
		{ActorTag::ENEMY_GREEN, score_enemy_green},
	};

	//バブルソート
	ScoreSet stock_a = {};
	ScoreSet stock_b = {};
	for (int i = 0; i < actorsCount; i++)
	{
		for (int j = i + 1; j < actorsCount; j++)
		{
			if (sets[i].score < sets[j].score)
			{
				stock_a = sets[i];
				stock_b = sets[j];
				sets[i] = stock_b;
				sets[j] = stock_a;
			}
		}
	}

	//順位登録
	for (int i = 0; i < actorsCount; i++)
	{
		switch (sets[i].tag)
		{
		case ActorTag::PLAYER:
			ranking[0] = i + 1;
			break;
		case ActorTag::ENEMY_RED:
			ranking[1] = i + 1;
			break;
		case ActorTag::ENEMY_GREEN:
			ranking[2] = i + 1;
			break;
		default:
			break;
		}
	}
}

