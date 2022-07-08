#include "Ending.h"
#include "Audio.h"
#include "Input.h"
#include "FBXManager.h"
#include "OBJLoader.h"

int Ending::killCount_player = 0;
int Ending::killCount_enemyA = 0;
int Ending::killCount_enemyB = 0;

Ending::Ending()
{
	next = Title;

	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());

	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	selectState = SelectState::Restart;

	for (int i = 0; i < enemyCount+1; i++)
	{
		set[i] = new ResultSet(i == 0);
	}

	sp_select = new Sprite();
	sp_restart = new Sprite();
	sp_title = new Sprite();
}


Ending::~Ending()
{
	for (int i = 0; i < enemyCount + 1; i++)
	{
		delete set[i];
	}

	delete sp_select;
	delete sp_restart;
	delete sp_title;
}

void Ending::Initialize()
{
	isEnd = false;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	selectState = SelectState::Restart;

	for (int i = 0; i < enemyCount + 1; i++)
	{
		int killCount = 0;
		if (i == 0)
			killCount = killCount_player;
		else if (i == 1)
			killCount = killCount_enemyA;
		else
			killCount = killCount_enemyB;
		set[i]->Initialize(killCount, positions_3d[i], positions_2d[i]);
	}
	topKillCount = 0;
	TopSearch();
	GaugeSize();

	pos_select = pos_restart;

	Audio::PlayWave("BGM_Result", 0.1f * Audio::volume_bgm, true);
}

void Ending::Update()
{
	SelectMenu();

	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->Update(topKillCount);
	}

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
		Audio::PlayWave("SE_Select", 1.0f * Audio::volume_se);
		selectState = SelectState::Restart;
		isSelectMove = true;
	}
	else if (Input::TriggerPadLStickRight())
	{
		Audio::PlayWave("SE_Select", 1.0f * Audio::volume_se);
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
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
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
		Audio::PlayWave("SE_Decision", 1.0f * Audio::volume_se);
		Audio::StopWave("BGM_Result");
		ShutDown();
	}
}

void Ending::TopSearch()
{
	bool isTops[enemyCount + 1] = { false,false,false };
	for (int i = 0; i < enemyCount+1; i++)
	{
		//0以下はスルー
		if (set[i]->killCount <= 0)
			continue;

		//記録更新
		if (topKillCount < set[i]->killCount)
		{
			topKillCount = set[i]->killCount;
			for (int j = 0; j < enemyCount+1; j++)
			{
				isTops[j] = false;
			}
			isTops[i] = true;
		}
		//同率
		else if (topKillCount == set[i]->killCount)
		{
			topKillCount = set[i]->killCount;
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
	int topCount = 0;
	for (int i = 0; i < enemyCount+1; i++)
	{
		if (set[i]->isTop)
		{
			set[i]->scaleY_gauge = 1.0f;
			topCount = set[i]->killCount;
		}
	}

	//
	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->scaleY_gauge = set[i]->killCount / topCount;
	}
}

Ending::ResultSet::ResultSet(const bool arg_isPlayer)
	:isPlayer(arg_isPlayer)
{
	std::string modelName = "GamePlay_Player";
	if (!isPlayer)
		modelName = "GamePlay_Enemy";

	object = Object3D::Create(FBXManager::GetModel(modelName), position, scale, rotation, color);
	obj_crown = Object3D::Create(OBJLoader::GetModel("Crown"), pos_crown, scale_crown, rotation_crown, color_crown);
	numberSprite = new NumberSprite(killCount_draw);
	sp_gauge = new Sprite();
}

Ending::ResultSet::~ResultSet()
{
	delete object;
	delete obj_crown;
	delete numberSprite;
	delete sp_gauge;
}

void Ending::ResultSet::Initialize(const int arg_killCount, const float arg_positionX_3d, const float arg_positionX_2d)
{
	killCount = arg_killCount;
	killCount_draw = 0.0f;
	position.x = arg_positionX_3d;
	pos_crown.x = arg_positionX_3d;
	positionX_2d = arg_positionX_2d;
	scaleY_gauge = 0.0f;
	scaleY_gauge_draw = 0.0f;
	isTop = false;
	drawCrown = false;
}

void Ending::ResultSet::Update(const int arg_topKillCount)
{
	//ゲージを伸ばす
	const bool gaugeExtend = scaleY_gauge_draw < scaleY_gauge;
	if (gaugeExtend)
	{
		scaleY_gauge_draw += 1.0f / 120.0f;
	}

	//数値を増やす
	const bool killCountAdd = killCount_draw < killCount;
	if (killCountAdd)
	{
		killCount_draw += arg_topKillCount / 120.0f;
	}

	//王冠の表示
	drawCrown = !gaugeExtend && !killCountAdd;

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

	const int digit = 2;
	numberSprite->Draw(digit, "GamePlay_UI_Number", { positionX_2d, 200.0f });

	std::string texName = "Result_Player_Gauge";
	if (!isPlayer)
		texName = "Result_Enemy_Gauge";
	sp_gauge->DrawSprite(texName, { positionX_2d, 770.0f }, 0.0f, { 1.0f, scaleY_gauge_draw }, { 1,1,1,1 }, { 0.5f, 1.0f });
}
