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

	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
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

	cameraDistance = 23.0f;
	camera.get()->SetDistance(cameraDistance);
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	for (int i = 0; i < panelsNum_ALL; i++)
	{
		panels[i]->Initialize();
	}
	//敷き詰め
	PanelPadding();

	//ランダムでボーナスパネルも配置

	isSceneChange = false;

	Audio::PlayWave("BGM_Title", 0.1f, true);
}

void Title::Update()
{
	//シーン切り替え
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		Audio::PlayWave("SE_Decision");
		//Audio::StopWave("BGM_Title");
		isSceneChange = true;
		//ShutDown();
	}

//#ifdef _DEBUG
	if (Input::TriggerKey(DIK_SPACE))
	{
		Audio::PlayWave("SE_Decision");
		//Audio::StopWave("BGM_Title");
		isSceneChange = true;
		//ShutDown();
	}
//#endif

	//シーン遷移
	if (isSceneChange)
	{
		if (PopUpPanel())
		{
			if(ZoomIn())
			{
				Audio::StopWave("BGM_Title");
				ShutDown();
			}
		}
	}

	//各更新
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

void Title::PanelPadding()
{
	int count_panel = 0;
	const float panelSize_X = 1.7f * 4.0f;
	const float panelSize_Y = 1.4f * 4.0f;
	Vector3 startPosition_panel = { -panelSize_X * (panelsNum_X / 2),-panelSize_Y * (panelsNum_Y / 2),0 };

	for (int i = 0; i < panelsNum_Y; i++)
	{
		for (int j = 0; j < panelsNum_X; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				panels[count_panel]->position.x = startPosition_panel.x + (panelSize_X * j) + ((panelSize_X / 2.0f) * k);
				panels[count_panel]->position.y = startPosition_panel.y + (panelSize_Y * i);
				//下向き
				if (k != 0)
				{
					panels[count_panel]->rotation.x = -90.0f;
				}
				//上向き
				else
				{
					panels[count_panel]->rotation.x = 90.0f;
					panels[count_panel]->rotation.z = 180.0f;
				}

				count_panel++;
			}
		}
	}
}

bool Title::PopUpPanel()
{
	//計算用に値を移す
	Vector3 position = panels[(panelsNum_ALL / 2) - 1]->position;
	//手前下方向に飛び出す
	const Vector3 velocity = { 0,-2,1 };
	position += velocity;
	//値を反映する
	panels[(panelsNum_ALL / 2) - 1]->position = position;

	//一定値以上落ちたら真を返す
	const float endPosition_Y = -50.0f;
	return position.y <= endPosition_Y;
}

bool Title::ZoomIn()
{
	//近づける
	const float speed = 1.0f;
	cameraDistance -= speed;
	//反映
	camera.get()->SetDistance(cameraDistance);


	//一定値以上近づいたら真を返す
	const float endDistance = 8.0f;
	return cameraDistance <= endDistance;
}

Title::Panel::Panel()
{
	object = Object3D::Create(OBJLoader::GetModel("fieldPiece_title"), position, scale, rotation, color);
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
	color = { 0.25f, 0.58f, 1.0f, 1.0f };
}
