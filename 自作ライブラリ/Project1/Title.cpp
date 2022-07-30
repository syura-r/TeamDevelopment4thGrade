#include "Title.h"

#include "DebugCamera.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "OBJLoader.h"
#include "Audio.h"
#include "Easing.h"
#include <random>

Title::Title()
{
	next = Play;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());
	Sprite3D::SetCamera(camera.get());

	//���C�g����
	lightGroup.reset(LightGroup::Create());
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	titleLogo = new Sprite3D();
	titleStart = new Sprite3D();

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i] = new Panel();
	}
}


Title::~Title()
{
	delete titleLogo;
	delete titleStart;
	for (int i = 0; i < panelsNum_ALL; i++)
	{
		delete panels[i];
	}
}

void Title::Initialize()
{
	isEnd = false;

	cameraDistance = cameraDistance_init;
	camera.get()->SetDistance(cameraDistance);
	Object3D::SetCamera(camera.get());
	Sprite3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	logoPosition = { 0.0f, logoPositionY_down, -20.0f };
	isLogoPositionUp = true;
	logoEasingCount = 0;

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->Initialize();
	}
	//�~���l��
	PanelPadding();

	velocity_pupUp = {};
	easingTimer_zoom = 0;
	isSceneChange = false;

	Audio::PlayBGM("BGM_Title", 0.1f * Audio::volume_bgm);
}

void Title::Update()
{
	//�V�[���J��
	if (isSceneChange)
	{
		if (PopUpPanel())
		{
			if(ZoomIn())
			{
				//�V�[���؂�ւ�
				Audio::StopBGM("BGM_Title");
				ShutDown();
			}
		}
	}
	else
	{
		//�V�[���؂�ւ��J�n
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
		{
			Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
			isSceneChange = true;
			velocity_pupUp = velocity_init;
		}
	}

	//
	LogoMove();

	//�e�X�V
	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->object->Update();
	}
	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,-1.0f,0.5f,1 });
	lightGroup->Update();
}

void Title::PreDraw()
{
	PipelineState::SetPipeline("BasicObj");

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->object->Draw();
	}

	const Vector3 pos_start = { 0.0f, -0.8f, -20.0f };
	const float scale_logo = 1.0f / 16.0f;
	const float scale_start = 1.0f / 22.0f;
	titleLogo->DrawSprite("titlelogo", logoPosition, 0.0f, { scale_logo, scale_logo });
	titleStart->DrawSprite("titlestart", pos_start, 0.0f, { scale_start, scale_start });
}

void Title::PostDraw()
{
}

float Title::GetRandom(float arg_min, float arg_max)
{
	std::random_device rnd;
	std::mt19937_64 mt64(rnd());
	std::uniform_real_distribution<float> genRandFloat(arg_min, arg_max);
	return genRandFloat(mt64);
}

void Title::PanelPadding()
{
	int count_panel = 0;
	const float panelSize_X = 1.7f * 4.0f;
	const float panelSize_Y = 1.5f * 4.0f;
	Vector3 startPosition_panel = { -panelSize_X * (panelsNum_X / 2),-panelSize_Y * (panelsNum_Y / 2),0 };

	for (int i = 0; i < panelsNum_Y; i++)
	{
		for (int j = 0; j < panelsNum_X; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				panels[count_panel]->position.x = startPosition_panel.x + (panelSize_X * j) + ((panelSize_X / 2.0f) * k);
				panels[count_panel]->position.y = startPosition_panel.y + (panelSize_Y * i);
				//������
				if (k != 0)
				{
					panels[count_panel]->rotation.x = -90.0f;
					panels[count_panel]->position.y += 1.0f;
				}
				//�����
				else
				{
					panels[count_panel]->rotation.x = 90.0f;
					panels[count_panel]->rotation.z = 180.0f;
					panels[count_panel]->position.y -= 1.0f;
				}

				count_panel++;
			}
		}
	}
}

bool Title::PopUpPanel()
{
	//�v�Z�p�ɒ����̃p�l���̒l���ڂ�
	Vector3 position = panels[(panelsNum_ALL / 2) - 1]->position;
	Vector3 rotation = panels[(panelsNum_ALL / 2) - 1]->rotation;

	//��O�������ɔ�яo��
	const float gravity = 0.1f;
	velocity_pupUp.y -= gravity;
	position += velocity_pupUp;

	//��]���t����
	rotation.x += -8.0f;

	//�l�𔽉f����
	panels[(panelsNum_ALL / 2) - 1]->position = position;
	panels[(panelsNum_ALL / 2) - 1]->rotation = rotation;

	//���l�ȏ㗎������^��Ԃ�
	const float endPosition_Y = -50.0f;
	return position.y <= endPosition_Y;
}

bool Title::ZoomIn()
{
	//�߂Â���
	const float endDistance = 7.0f;
	const int easingLimit = 45;
	cameraDistance = Easing::EaseInCirc(cameraDistance_init, endDistance, easingLimit, easingTimer_zoom);
	easingTimer_zoom++;
	//���f
	camera.get()->SetDistance(cameraDistance);

	//�ڕW�n�_�܂ŋ߂Â�����^��Ԃ�
	return cameraDistance <= endDistance;
}

void Title::LogoMove()
{
	const int limit = 120;
	if (isLogoPositionUp)
	{
		logoPosition.y = Easing::EaseOutSine(logoPositionY_down, logoPositionY_up, limit, logoEasingCount);
	}
	else
	{
		logoPosition.y = Easing::EaseOutSine(logoPositionY_up, logoPositionY_down, limit, logoEasingCount);
	}
	logoEasingCount++;
	if (logoEasingCount >= limit)
	{
		logoEasingCount = 0;
		isLogoPositionUp = !isLogoPositionUp;
	}
}

Title::Panel::Panel()
{
	object = Object3D::Create(OBJLoader::GetModel("fieldPiece"), position, scale, rotation, color);
}

Title::Panel::~Panel()
{
	delete object;
}

void Title::Panel::Initialize()
{
	position = { 0,0,-15 };
	scale = { 4,4,4, };
	rotation = { 0,0,0 };
	color = { 1,1,1,1 };
}
