#include "Play.h"
#include <fstream>
#include <sstream>
#include"Object3D.h"
#include"Audio.h"
#include"ParticleEmitter.h"
#include"ParticleManager.h"
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
#include "BaseGameActor.h"

#include "TextureResource.h"
#include "StandardEnemy.h"
#include "ItemEmitter.h"
#include "Stadium.h"
#include "PtrDelete.h"
#include "ScreenCamera.h"
#include "InGameCamera.h"
#include "IEnemyAI.h"

Play::Play()
{
	next = Ending;
	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());
	Sprite3D::SetCamera(camera.get());

	screenCamera = new ScreenCamera();
	
	//ParticleEmitter::Initialize(camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());
	Player::SetCamera(camera.get());
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();
	actorManager = ActorManager::GetInstance();
	actorManager->Initialize();

	objectManager->AddObjectsAtOnce();

	pause = new Pause();
	timeLimit = new TimeLimit(120 * 60);//制限時間の設定はここ
	feverUI = new FeverUI();
	levelGauge = new LevelGauge();
	lockonMarker = new LockonMarker();
	scoreRanking = new ScoreRanking();

	screenResource = new TextureResource("screen.png", false, true, { 480,270 });
	stadium = new Stadium();


	ParticleEmitter::SetObjectManager(objectManager);
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
	PtrDelete(pause);
	PtrDelete(timeLimit);
	PtrDelete(feverUI);
	PtrDelete(levelGauge);
	PtrDelete(lockonMarker);
	PtrDelete(scoreRanking);
	PtrDelete(stadium);
	PtrDelete(screenResource);
	PtrDelete(screenCamera);
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

	Player* player = new Player(Vector3(0, -5, 10));
	objectManager->Add(player);	
	
	StandardEnemy* testEnemy = new StandardEnemy(Vector3(-10, -5, -10), EnemyAILabel::POSITIVE, "GamePlay_Enemy");
	objectManager->Add(testEnemy);	

	StandardEnemy* testEnemy2 = new StandardEnemy(Vector3(10, -5, -10), EnemyAILabel::POSITIVE, "GamePlay_Enemy2");
	objectManager->Add(testEnemy2);

	camera->Initialize();
	stadium->Initialize();

	screenCamera->SetTargetObj(player);

	ItemEmitter::GetInstance()->Initialize();

	isEnd = false;
	pause->Initialize();
	timeLimit->Initialize();
	gameEndCount = 0;
	feverUI->Initialize();
	levelGauge->Initialize();
	lockonMarker->Initialize();
	scoreRanking->Initialize();

	Audio::StopBGM("BGM_Play");
	Audio::PlayBGM("BGM_Play", 0.1f * Audio::volume_bgm);
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
		Audio::StopBGM("BGM_Play");
		next = Title;
		KillCountToEnding();
		ShutDown();
		return;
	}
	//ポーズ画面を開いているとき
	if (pause->GetActivePause())
	{
		//BGMの音量変更
		Audio::VolumeChangeBGM("BGM_Play", 0.1f * Audio::volume_bgm);
		return;
	}


#ifdef _DEBUG
	if (Input::TriggerKey(DIK_E))//終了処理
	{
		Audio::StopBGM("BGM_Play");
		KillCountToEnding();
		ShutDown();
		return;
	}
#endif
	
	timeLimit->Update();
	if (timeLimit->GetLimit())
	{
		gameEndCount++;

		if (gameEndCount >= 60)
		{
			Audio::StopBGM("BGM_Play");
			Audio::AllStopSE();

			KillCountToEnding();
			ShutDown();
		}

		return;
	}

	// ゲームパッドの右スティックでのカメラ操作
	if (Input::CheckPadRStickLeft() || Input::CheckPadRStickUp() || Input::CheckPadRStickRight() || Input::CheckPadRStickDown())
	{
		Vector2 vec;
		vec.x = Input::GetRStickDirection().x;
		vec.y = Input::GetRStickDirection().y;
		camera->RotateYaxis(vec);
	}

	feverUI->Update();
	levelGauge->Update();
	scoreRanking->Update();

	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	ItemEmitter::GetInstance()->Update();
	objectManager->Update();
	stadium->Update();
	camera->Update();
	screenCamera->Update();

	actorManager->CollisionCheck();
	collisionManager->CheckAllCollisions();

	//プレイヤーの押し付け先
	BaseGameActor* targetActor = actorManager->GetPlayer()->GetTarget();
	if (targetActor)
	{
		lockonMarker->SetParentPosition(targetActor->GetPosition());
		lockonMarker->SetIsActive(true);
	}
	else
	{
		lockonMarker->SetIsActive(false);
	}

	/*if (ActorManager::GetInstance()->GetPlayer()->IsEndGame() )
	{
		Audio::StopWave("BGM_Play");
		KillCountToEnding();
		ShutDown();
		return;
	}

	bool allEnemiesOutField = true;
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto e : enemies)
	{
		if (!e->IsEndGame())
		{
			allEnemiesOutField = false;
			break;
		}
	}
	if (allEnemiesOutField)
	{
		Audio::StopWave("BGM_Play");
		KillCountToEnding();
		ShutDown();
		return;
	}*/
}

void Play::PreDraw()
{
	timeLimit->Draw();
	feverUI->Draw();
	levelGauge->Draw();

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
		scoreRanking->Draw_OBJ();
		Object3D::SetScreenDraw(false);
		Sprite3D::SetCamera(camera.get());
		Object3D::SetCamera(camera.get());
		screenResource->PostDraw();

		objectManager->PreDraw();
		scoreRanking->Draw_OBJ();
		stadium->Draw();
		ParticleManager::GetInstance()->DrawFeverCutEffect();
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;

	objectManager->PostDraw();
	if (!Object3D::GetDrawShadow())
	{
		DirectXLib::GetInstance()->DepthClear();
	}
	lockonMarker->Draw();
	pause->Draw();
}

void Play::TimeUpdate()
{
}

void Play::KillCountToEnding()
{
	Ending::score_player = actorManager->GetPlayer()->GetTotalCutCount();
	Ending::score_enemy_red = actorManager->GetStandardEnemies()[0]->GetTotalCutCount();
	Ending::score_enemy_green = actorManager->GetStandardEnemies()[1]->GetTotalCutCount();
}
