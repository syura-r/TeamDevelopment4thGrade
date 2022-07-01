#include "Title.h"

#include "DebugCamera.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "OBJLoader.h"
#include "Audio.h"

Title::Title()
{
	next = Play;

	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());

	//���C�g����
	lightGroup.reset(LightGroup::Create());
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	titleLogo = new Sprite();
	titleStart = new Sprite();

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

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	int count_panel = 0;
	const float panelSize_X = 1.7f;
	const float panelSize_Y = 1.4f;
	Vector3 startPosition_panel = { -panelSize_X * (panelsNum_X / 2),-panelSize_Y * (panelsNum_Y / 2),0 };

	//�~���l��
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
				}
				//�����
				else
				{
					panels[count_panel]->rotation.x = 90.0f;
					panels[count_panel]->rotation.z = 180.0f;
				}

				count_panel++;
			}
		}
	}


	Audio::PlayWave("BGM_Title", 0.1f, true);
}

void Title::Update()
{
	//�V�[���؂�ւ�
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");
		Audio::StopWave("BGM_Title");
		ShutDown();
	}

//#ifdef _DEBUG
	if (Input::TriggerKey(DIK_SPACE))
	{
		Audio::PlayWave("SE_Decision");
		Audio::StopWave("BGM_Title");
		ShutDown();
	}
//#endif

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->object->Update();
	}
	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,-1.0f,0.2f,1 });
	lightGroup->Update();
}

void Title::PreDraw()
{
	PipelineState::SetPipeline("BasicObj");

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->object->Draw();
	}
}

void Title::PostDraw()
{
	const Vector2 windowCenterPosition = { 960,500 };
	const float startcharPlusPositionY = 280;

	const float scale = 2.0f;
	titleLogo->DrawSprite("titlelogo", windowCenterPosition, 0.0f, { scale, scale });
	titleStart->DrawSprite("titlestart", { windowCenterPosition.x, windowCenterPosition.y + startcharPlusPositionY }, 0.0f, { 1.5f, 1.5f });
}

Title::Panel::Panel()
{
	object = Object3D::Create(OBJLoader::GetModel("fieldPiece_title"), position, scale, rotation, color);
}

Title::Panel::~Panel()
{
	delete object;
}
