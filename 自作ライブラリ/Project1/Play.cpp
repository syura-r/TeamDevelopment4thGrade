#include "Play.h"
#include <fstream>
#include <sstream>
#include"Object3D.h"
#include"Audio.h"
#include"ParticleEmitter.h"
#include "DrawMode.h"
#include"imgui.h"
#include"TouchAbleObject.h"
#include"FBXManager.h"
#include "Input.h"
#include "MapBox.h"
#include "Easing.h"
#include "LevelEditor.h"
#include "Player.h"
#include "Floor.h"

Play::Play()
{
	next = Ending;
	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());
	ParticleEmitter::Initialize(camera.get());
	Player::SetDebugCamera(camera.get());
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));
	menu = std::make_unique<Menu>();
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();
	player = new Player();
	objectManager->Add(player);
	secondTimeSprite = std::make_unique<NumberSprite>(secondTime);
	minuteTimeSprite = std::make_unique<NumberSprite>(minuteTime);

	pauseBackTex = std::make_unique<Sprite>();
	halfPauseBackTex = std::make_unique<Sprite>();
	quarterPauseBackTex = std::make_unique<Sprite>();
	colon = std::make_unique<Sprite>();
	resource.reset(new TextureResource("PauseBack",{1920,1080}, DXGI_FORMAT_R8G8B8A8_UNORM,{1,1,1,1},false));
	halfResource.reset(new TextureResource("halfPauseBack",{960,540}, DXGI_FORMAT_R8G8B8A8_UNORM, { 1,1,1,1 }, false));
	quarterResource.reset(new TextureResource("quarterPauseBack",{480,270}, DXGI_FORMAT_R8G8B8A8_UNORM, { 1,1,1,1 }, false));

	result = std::make_unique<Result>();
//---------------------------------仮実装------------------------------------------
	std::string filepath = "Resources/Map/Report" + std::to_string(0) + ".txt";
	std::ifstream file;
	file.open(filepath.c_str());
	if (!file.is_open())
		return;
	std::string line;
	std::vector<Vector3>positions;
	std::vector<Vector3>scales;
	std::vector<Vector3>rotations;
	std::vector<Object*>loadObjects;
	objectManager->Add(new Floor());
	for (int i = 0; i < 9; i++)
	{
		lightGroup->SetPointLightActive(i, true);
		lightGroup->SetPointLightPos(i, lightPos[i].data() - Vector3{ 0,3,0 });
	}

	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字を取得
		std::string key;
		getline(line_stream, key, ' ');

		//クラス名
		if (key == "class")
		{
			std::string name;
			line_stream >> name;
		}
		//ポジション
		if (key == "position")
		{
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.push_back(position);
		}
		//スケール
		if (key == "scale")
		{
			Vector3 scale;
			line_stream >> scale.x;
			line_stream >> scale.y;
			line_stream >> scale.z;

			scales.push_back(scale);
		}
		//回転
		if (key == "rotation")
		{
			Vector3 rotation;
			line_stream >> rotation.x;
			line_stream >> rotation.y;
			line_stream >> rotation.z;

			rotations.push_back(rotation);
		}
	}
	file.close();

	int i = 0;
	for (auto& it : loadObjects) {
		it->SetPosition(positions[i]);
		it->SetScale(scales[i]);
		it->SetRotation(rotations[i]);
		ObjectManager::GetInstance()->Add(it);
		i++;
	}
//---------------------------------------------------------------------------
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
}

void Play::Initialize()
{
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());
	objectManager->Initialize();
	isEnd = false;
	pause = false;
	pauseFrame = false;
	minuteTime = 0;
	secondTime = 0;
	onResult = false;
	getAchieve = 0;
}

void Play::Update()
{
	if(onResult)
	{
		if(result->Update())
		{
			if(result->GetRetry())
			{
				next = SCENE::Play;
			}
			if (result->GetReturnTitle())
			{
				next = SCENE::Title;
			}
			isEnd = true;
		}
		return;
	}
	if (pause)
	{
		if (menu->Update())
		{
			if (menu->GetRestart())
			{
				player->Reset();
				frameCounter = 0;
				secondTime = 0;
				minuteTime = 0;
			}
			if(menu->GetReturnTitle())
			{
				next = SCENE::Title;
				isEnd = true;
			}
			pause = false;
		}
		return;
	}
	camera->Update();
	if (Input::TriggerKey(DIK_ESCAPE) || Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		menu->Initialize();
		pause = true;
		pauseFrame = true;
		return;
	}

	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	objectManager->Update();
	collisionManager->CheckAllCollisions();
	if (!player->GetReset())
		TimeUpdate();
}

void Play::PreDraw()
{
	if (pauseFrame && !Object3D::GetDrawShadow())
		resource->PreDraw();
	if ((!pause && !onResult) || pauseFrame)
	{
		objectManager->DrawReady();
#ifdef _DEBUG
		if (DrawMode::GetDrawImGui() && Object3D::GetDrawShadow())
		{
			ImGui::Begin("Light");
			ImGui::SliderFloat3("LightDir", lightDir, -1.0f, 1.0f);

			ImGui::End();
			Object3D::GetLightCamera()->SetLightDir({ lightDir[0],lightDir[1] ,lightDir[2] });
			LevelEditor::GetInstance()->Draw();
		}
#endif
		objectManager->PreDraw();
		if (!Object3D::GetDrawShadow())
		{
			secondTimeSprite->Draw(2, "number", { 1650,80 }, { 1,1 }, { 1,1,1,1 }, { 0,0.5f });
			int digits = 1;
			if (minuteTime >= 10) digits = 2;
			minuteTimeSprite->Draw(digits, "number", { 1550,80 }, { 1,1 }, { 1,1,1,1 }, { 1,0.5f });
			colon->DrawSprite("colon", { 1600,80 });
		}
	}
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;
	if (!pause || pauseFrame)
		objectManager->PostDraw();
	if (pauseFrame && !Object3D::GetDrawShadow())
	{
		resource->PostDraw(false);
		
		halfResource->PreDraw(1,0, 0, 960, 540);
		pauseBackTex->DrawSprite("PauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		halfResource->PostDraw(false);
		
		quarterResource->PreDraw(1,0, 0, 960, 540);
		halfPauseBackTex->DrawSprite("halfPauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 },"Blur");
		quarterResource->PostDraw(false);

		halfResource->PreDraw();
		quarterPauseBackTex->DrawSprite("quarterPauseBack", { 0,0 }, 0, {2,2 }, { 1,1,1,1 }, { 0,0 }, "Blur");
		halfResource->PostDraw(false);

		resource->PreDraw(1,0,0,1920*2,1080*2);
		halfPauseBackTex->DrawSprite("halfPauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		resource->PostDraw();
		
		pauseFrame = false;
	}
	if ((pause || onResult) && !Object3D::GetDrawShadow())
	{
		pauseBackTex->DrawSprite("PauseBack", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 }, "NoAlphaToCoverageSprite");
		DirectXLib::GetInstance()->DepthClear();
	}
	if (pause && !Object3D::GetDrawShadow() && !isEnd)
	{
		menu->Draw();
	}
	if (onResult && !Object3D::GetDrawShadow() && !isEnd)
	{
		result->Draw();
	}

}

void Play::TimeUpdate()
{
	if(Input::TriggerKey(DIK_P))
	{
		minuteTime += 1.0f;
	}
	frameCounter++;
	if(frameCounter >= 60)
	{
		frameCounter = 0;
		secondTime += 0.1f;
		if(secondTime>=6)
		{
			secondTime = 0;
			minuteTime+=1.0f;
		}
	}
}
