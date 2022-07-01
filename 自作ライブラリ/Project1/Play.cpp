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
#include "Field.h"
#include "Ending.h"
#include "ScoreManager.h"

#include "TextureResource.h"
#include "StandardEnemy.h"
#include "ItemEmitter.h"
#include "Stadium.h"
#include "PtrDelete.h"
#include"ScreenCamera.h"
Play::Play()
{
	next = Ending;
	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());
	Sprite3D::SetCamera(camera.get());

	screenCamera = new ScreenCamera();
	
	//ParticleEmitter::Initialize(camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());
	Player::SetDebugCamera(camera.get());
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));
	//menu = std::make_unique<Menu>();
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();
	actorManager = ActorManager::GetInstance();
	actorManager->Initialize();

	//result = std::make_unique<Result>();
	objectManager->AddObjectsAtOnce();

	pause = new Pause();
	timeLimit = new TimeLimit(180 * 60);//制限時間の設定はここ
	scoreUI = new ScoreUI();

	screenResource = new TextureResource("screen.png", false, true, { 480,270 });
	stadium = new Stadium();

	//test = new Sprite3D();

	ParticleEmitter::SetObjectManager(objectManager);
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
	delete pause;
	delete timeLimit;
	delete scoreUI;
	PtrDelete(stadium);
	PtrDelete(screenResource);
	PtrDelete(screenCamera);
	//PtrDelete(test);
	ScoreManager::GetInstance()->Finalize();
}

void Play::Initialize()
{
	next = Ending;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	objectManager->Reset();
	actorManager->Initialize();

	//ゲームオブジェクト生成
	Field* field = new Field();
	objectManager->Add(field, false);	

	Player* player = new Player();
	objectManager->Add(player);	
	
	StandardEnemy* testEnemy = new StandardEnemy();
	objectManager->Add(testEnemy);	

	StandardEnemy* testEnemy2 = new StandardEnemy();
	objectManager->Add(testEnemy2);

	stadium->Initialize();

	screenCamera->SetTargetObj(player);

	ItemEmitter::GetInstance()->Initialize();

	isEnd = false;
	pause->Initialize();
	timeLimit->Initialize();
	gameEndCount = 0;
	scoreUI->Initialize();

	ScoreManager::GetInstance()->Inisitlize();

	Audio::PlayWave("BGM_Play", 0.1f, true);
}

void Play::Update()
{
	pause->Update();
	//ゲームにもどる
	if (pause->GetToGame())
	{
		return;
	}
	//やり直す
	if (pause->GetRestart())
	{
		Initialize();
		return;
	}
	//タイトルにもどる
	if (pause->GetToTitle())
	{
		Audio::StopWave("BGM_Play");
		next = Title;
		ShutDown();
		return;
	}
	//ポーズ画面を開いているとき
	if (pause->GetActivePause())
		return;


#ifdef _DEBUG
	if (Input::TriggerKey(DIK_E))//終了処理
	{
		Audio::StopWave("BGM_Play");
		ShutDown();
		return;
	}
#endif
	
	timeLimit->Update();
	if (timeLimit->GetLimit())
	{
		//Field* field = actorManager->GetFields()[0];
		////Fieldに指令出す

		gameEndCount++;

		if (gameEndCount >= 60)
		{
			ShutDown();
		}

		return;
	}
	scoreUI->Update();
	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	ItemEmitter::GetInstance()->Update();
	objectManager->Update();
	stadium->Update();
	camera->Update();
	screenCamera->Update();

	collisionManager->CheckAllCollisions();

	if (ActorManager::GetInstance()->GetPlayer()->IsGameEnd() )
	{
		Audio::StopWave("BGM_Play");
		ShutDown();
		return;
	}

	bool allEnemiesOutField = true;
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto e : enemies)
	{
		if (!e->GetOutField())
		{
			allEnemiesOutField = false;
			break;
		}
	}
	if (allEnemiesOutField)
	{
		Audio::StopWave("BGM_Play");
		ShutDown();
		return;
	}
}

void Play::PreDraw()
{
		objectManager->DrawReady();
#ifdef _DEBUG
		if (DrawMode::GetDrawImGui() && !Object3D::GetDrawShadow())
		{
			ImGui::Begin("Light");
			ImGui::SliderFloat3("LightDir", lightDir, -1.0f, 1.0f);

			ImGui::End();
			Object3D::GetLightCamera()->SetLightDir({ lightDir[0],lightDir[1] ,lightDir[2] });
			LevelEditor::GetInstance()->Draw();
		}
#endif

		screenResource->PreDraw(1,0,0,480,270,0,0,480,270);
		Object3D::SetCamera(screenCamera);
		Sprite3D::SetCamera(screenCamera);
		Object3D::SetScreenDraw(true);
		objectManager->PreDraw();
		Object3D::SetScreenDraw(false);
		Sprite3D::SetCamera(camera.get());
		Object3D::SetCamera(camera.get());
		screenResource->PostDraw();

		objectManager->PreDraw();
		stadium->Draw();
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;
	scoreUI->Draw();
	timeLimit->Draw();
	pause->Draw();

	objectManager->PostDraw();

	static Vector3 pos = { 0,0,0 };

	if (Input::DownKey(DIK_RIGHT))
	{
		pos.x += 0.3f;
	}
	if (Input::DownKey(DIK_LEFT))
	{
		pos.x -= 0.3f;
	}
	if (Input::DownKey(DIK_UP))
	{
		pos.z += 0.3f;
	}
	if (Input::DownKey(DIK_DOWN))
	{
		pos.z -= 0.3f;
	}
	if (Input::DownKey(DIK_K))
	{
		pos.y -= 0.3f;
	}
	if (Input::DownKey(DIK_I))
	{
		pos.y += 0.3f;
	}
	//test->SpriteSetTextureRect("stadiumMap.png", 100, 100, 300, 300);
	//test->DrawSprite("stadiumMap.png", pos);
	if (!Object3D::GetDrawShadow())
	{
		DirectXLib::GetInstance()->DepthClear();
	}

}

void Play::TimeUpdate()
{
}
