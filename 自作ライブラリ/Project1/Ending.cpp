#include "Ending.h"
#include "Audio.h"
#include "Input.h"
#include "FBXManager.h"
#include "OBJLoader.h"

int Ending::killCount_player = 0;
int Ending::killCount_enemy_red = 0;
int Ending::killCount_enemy_green = 0;

Ending::Ending()
{
	next = Title;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());

	//���C�g����
	lightGroup.reset(LightGroup::Create());
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
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
		int killCount = 0;
		if (i == 0)
			killCount = killCount_player;
		else if (i == 1)
			killCount = killCount_enemy_red;
		else
			killCount = killCount_enemy_green;
		set[i]->Initialize(killCount, positions_3d[i], positions_2d[i]);
	}
	topKillCount = 0;
	TopSearch();
	GaugeSize();
	isSkipOrFinish = false;

	pos_select = pos_title;

	Audio::PlayBGM("BGM_Result", 0.1f * Audio::volume_bgm);

	stadium->Initialize();
	stadium->SetPosition({ 0.0f,-40.0f,0.0f });
}

void Ending::Update()
{
	SelectMenu();

	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->Update(topKillCount);
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

	//�I������
	sp_title->DrawSprite("toTitle", pos_title);
	sp_restart->DrawSprite("restart", pos_restart);

	sp_select->DrawSprite("white1x1", pos_select, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,1 });

	stadium->Draw();
}

void Ending::PostDraw()
{
}

void Ending::SelectMenu()
{
	bool isSelectMove = false;//�I����ς�����

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

	//�J�[�\���ʒu
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
	}

	//�V�[���؂�ւ�
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

	//�X�L�b�v
	MotionSkip();
}

void Ending::TopSearch()
{
	bool isTops[enemyCount + 1] = { false,false,false };
	for (int i = 0; i < enemyCount+1; i++)
	{
		//0�ȉ��̓X���[
		if (set[i]->killCount <= 0)
			continue;

		//�L�^�X�V
		if (topKillCount < set[i]->killCount)
		{
			topKillCount = set[i]->killCount;
			for (int j = 0; j < enemyCount+1; j++)
			{
				isTops[j] = false;
			}
			isTops[i] = true;
		}
		//����
		else if (topKillCount == set[i]->killCount)
		{
			topKillCount = set[i]->killCount;
			isTops[i] = true;
		}
	}

	//���f
	for (int i = 0; i < enemyCount + 1; i++)
	{
		set[i]->isTop = isTops[i];
	}
}

void Ending::GaugeSize()
{
	//��ʂ���i1.0f�j�ɂ���
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
			set[i]->killCount_draw = set[i]->killCount;
			set[i]->scaleY_gauge_draw = set[i]->scaleY_gauge;
		}
	}
	else
	{
		//�S�ẴQ�[�W�Ɛ��l�̓������I�������^
		isSkipOrFinish = true;
		for (int i = 0; i < enemyCount + 1; i++)
		{
			isSkipOrFinish =
				set[i]->killCount_draw >= set[i]->killCount &&
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
	//�Q�[�W��L�΂�
	const bool gaugeExtend = scaleY_gauge_draw < scaleY_gauge;
	if (gaugeExtend)
	{
		scaleY_gauge_draw += 1.0f / 120.0f;
	}

	//���l�𑝂₷
	const bool killCountAdd = killCount_draw < killCount;
	if (killCountAdd)
	{
		killCount_draw += arg_topKillCount / 120.0f;
	}

	//�����̕\��
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
