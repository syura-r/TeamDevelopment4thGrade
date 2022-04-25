#include"Game.h"
#include"FPS.h"
#include"OBJLoader.h"
#include"Audio.h"
#include"ParticleEmitter.h"
#include"DebugText.h"
#include"FbxLoader.h"
#include"Object3D.h"
#include"Create3DObject.h"
#include"Texture.h"
#include"Scene.h"
#include"Title.h"
#include"Play.h"
#include"Alpha.h"
#include"Ending.h"
#include"PipelineState.h"
#include"FBXManager.h"
#include"DrawMode.h"
#include "ComputeShade.h"
#include "ComputeWrapper.h"
#include "GameSettingParam.h"
#include "LevelEditor.h"
#include"window.h"
#include"DirectXLib.h"
#include "LightCamera.h"
#include"SceneManager.h"
#include "Sprite.h"
#include "TextureResource.h"
DrawMode::MODE DrawMode::mode = DrawMode::None;
bool DrawMode::drawImGui = true;
TextureResource* TextureResource::mainResource = nullptr;
Game::Game()
{
	win = std::make_unique<Window>(1920,1080);
	directX = DirectXLib::GetInstance();
	computeWrapper = ComputeWrapper::GetInstance();
}

Game * Game::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Game();
	}
	return instance;
}

Game::~Game()
{
}

void Game::RoadAsset()
{
	switch (loadAssetLevel)
	{
	case 0:
		//テクスチャの初期化
		Texture::Initialize();
		//テクスチャの読み込み
		Texture::LoadTexture("LoadDot", "LoadDot.png");
		Texture::LoadTexture("LoadPicture", "LoadPicture.png");
		break;
	case 1:
		Texture::LoadTexture("Debug", "ASCII.png");
		Texture::LoadTexture("particle", "particle1.png");
		Texture::LoadTexture("shock", "shock.png");
		Texture::LoadTexture("dust", "dust.png");
		Texture::LoadTexture("white1x1", "white1x1.png");
		Texture::LoadTexture("SampleNormalMap", "SampleNormalMap.png");
		Texture::LoadTexture("StainedGlass", "StainedGlass.png");
		Texture::LoadTexture("StainedGlassNormal", "StainedGlassNormal.png");
		Texture::LoadTexture("menuUI", "menuUI.png");
		Texture::LoadTexture("end", "end.png");
		Texture::LoadTexture("setting", "setting.png");
		Texture::LoadTexture("restart", "restart.png");
		Texture::LoadTexture("no", "no.png");
		Texture::LoadTexture("yes", "yes.png");
		Texture::LoadTexture("endCheck", "endCheck.png");
		Texture::LoadTexture("operateSetting", "operateSetting.png");
		Texture::LoadTexture("dot", "dot.png");
		Texture::LoadTexture("restartCheck", "restartCheck.png");
		Texture::LoadTexture("number", "number.png");
		Texture::LoadTexture("resultNumber", "resultNumber.png");

		Texture::LoadTexture("sensitiveSettingBar", "sensitiveSettingBar.png");
		Texture::LoadTexture("sensitiveSetting", "sensitiveSetting.png");

		Texture::LoadTexture("check", "check.png");
		Texture::LoadTexture("jump", "jump.png");
		Texture::LoadTexture("cameraReset", "cameraReset.png");
		Texture::LoadTexture("action", "action.png");
		Texture::LoadTexture("upsideDown", "upsideDown.png");
		Texture::LoadTexture("changeRun", "changeRun.png");
		Texture::LoadTexture("flipLeftRight", "flipLeftRight.png");
		Texture::LoadTexture("x", "x.png");
		Texture::LoadTexture("b", "b.png");
		Texture::LoadTexture("y", "y.png");
		Texture::LoadTexture("a", "a.png");
		Texture::LoadTexture("LB", "LB.png");
		Texture::LoadTexture("RT", "RT.png");
		Texture::LoadTexture("LT", "LT.png");
		Texture::LoadTexture("RB", "RB.png");
		Texture::LoadTexture("selectFrame", "selectFrame.png"); 
		Texture::LoadTexture("viewCollision", "viewCollision.png");
		Texture::LoadTexture("SSAO", "SSAO.png");
		Texture::LoadTexture("screenSetting", "ScreenSetting.png");
		Texture::LoadTexture("colon", "colon.png");
		Texture::LoadTexture("resultColon", "resultColon.png");

		Texture::LoadTexture("star", "star.png"); 
		Texture::LoadTexture("achieve", "achieve.png");
		Texture::LoadTexture("resultNumber", "resultNumber.png");
		Texture::LoadTexture("result", "result.png");
		Texture::LoadTexture("half", "half.png");
		Texture::LoadTexture("retry", "retry.png");
		Texture::LoadTexture("returnTitle", "returnTitle.png");
		Texture::LoadTexture("1MinuteClear", "1MinuteClear.png");
		Texture::LoadTexture("clearTime", "clearTime.png");

		Texture::LoadTexture("wind", "wind.png");
		Texture::LoadTexture("starEffect", "starEffect.png");

		Texture::LoadTexture("titleStart", "titleStart.png");
		Texture::LoadTexture("titleEnd", "titleEnd.png");

		//Texture::LoadTexture("particle2", "Particle_Soft.png");

		
		break; 
	case 2:
		//Objモデルの読み込み
		OBJLoader::LoadModelFile("box", "box.obj", false);
		OBJLoader::LoadModelFile("sphere", "sphere.obj", true);
		OBJLoader::LoadModelFile("ground", "ground.obj", false);
		OBJLoader::LoadModelFile("barrier", "Barrier.obj", false);
		OBJLoader::LoadModelFile("plain", "plain.obj", false);
		OBJLoader::LoadModelFile("chr_sword", "chr_sword.obj", true);
		break;
	case 3:
		//Objモデルの読み込み
		//OBJLoader::LoadModelFile("wall", "Wall.obj", false);
		OBJLoader::LoadModelFile("backWall", "BackWall.obj", false);
		OBJLoader::LoadModelFile("leftWall", "LeftWall.obj", false);
		OBJLoader::LoadModelFile("rightWall", "RightWall.obj", false);
		OBJLoader::LoadModelFile("frontWall", "FrontWall.obj", false);
		OBJLoader::LoadModelFile("blackBoard", "BlackBoard.obj", false);
		OBJLoader::LoadModelFile("book1", "book1.obj", false);
		OBJLoader::LoadModelFile("book2", "book2.obj", false);
		OBJLoader::LoadModelFile("book3", "book3.obj", false);


		OBJLoader::LoadModelFile("ceiling", "Ceiling.obj", false);
		OBJLoader::LoadModelFile("locker", "Rocker.obj", false);
		OBJLoader::LoadModelFile("cleaningToolStorage", "cleaningToolStorage.obj", false);
		OBJLoader::LoadModelFile("setSquare30", "SetSquare30.obj", false);
		OBJLoader::LoadModelFile("setSquare45", "SetSquare45.obj", false);
		OBJLoader::LoadModelFile("protractor", "Protractor.obj", false);
		OBJLoader::LoadModelFile("Fluorescent", "light.obj", false);

		OBJLoader::LoadModelFile("chair", "chair.obj", false);
		OBJLoader::LoadModelFile("desk", "desk.obj", false);
		OBJLoader::LoadModelFile("eraser", "eraser.obj", true);
		OBJLoader::LoadModelFile("pencil", "pencil.obj", true);
		OBJLoader::LoadModelFile("teacherDesk", "teachingTable.obj", false);
		OBJLoader::LoadModelFile("floor", "floor.obj", true);
		OBJLoader::LoadModelFile("sky", "sky.obj", true);
		OBJLoader::LoadModelFile("sun", "sun.obj", true);
		OBJLoader::LoadModelFile("Area", "Area.obj", true);


		break;
	case 4:
		//FBXファイルの読み込み
		FBXManager::LoadModelFile("player", "player", false);
		FBXManager::LoadModelFile("cleaningToolStorage", "cleanbox", false);
		//WAVファイルの読み込み
		//Audio::LoadFile("test", "51e7ff6cc6b93897.wav");
		break;
	case 5:
		//FBXアニメーションの登録
		FBXManager::AddAnimationList("player", "walk",0,60);
		FBXManager::AddAnimationList("player", "stand", 61, 61);
		FBXManager::AddAnimationList("player", "run", 62, 121);
		FBXManager::AddAnimationList("player", "jump", 122, 152);
		FBXManager::AddAnimationList("player", "secondJump", 153, 180);
		FBXManager::AddAnimationList("player", "airSlide", 181, 190);
		FBXManager::AddAnimationList("cleaningToolStorage", "open", 0,60);

		break;
	default:
		loadAssetFinish = true;
		break;
	}
	loadAssetLevel++;
}

void Game::CreatePipeline()
{
	switch (createPipelineLevel)
	{
	case 0:
		PipelineState::CreatePipeline("Sprite", SPRITE);
		PipelineState::CreatePipeline("DepthSprite", DepthSPRITE);

		break;
	case 1:
		PipelineState::CreatePipeline("FBX", FBX);
		PipelineState::CreatePipeline("BasicObj", BasicObj);
		break;
	case 2:
		PipelineState::CreatePipeline("Particle", PARTICLE, ADD);
		//PipelineState::CreatePipeline("Bloom", BasePostEffect);
		PipelineState::CreatePipeline("FBXPolygonBreak", FBXPolygonBreak);

		
		break;
	case 3:
		PipelineState::CreatePipeline("Bloom", Bloom);
		PipelineState::CreatePipeline("NoShade", NoShade);
		break;
	case 4:
		PipelineState::CreatePipeline("CollisionBox", ViewCollisionBox);
		PipelineState::CreatePipeline("CollisionSphere", ViewCollisionSphere);
		//PipelineState::CreatePipeline("NormalMap", NormalMap);
		break;
	case 5:
		PipelineState::CreatePipeline("PolygonBreak", PolygonBreak);
		PipelineState::CreatePipeline("BloomObj", BloomObj);

		break;
	case 6:
		PipelineState::CreatePipeline("FBXShadowMap", FBXShadowMap);
		PipelineState::CreatePipeline("Sky", SkyDome);

		break;
	case 7:
		PipelineState::CreatePipeline("ShadowMap", NormalShadowMap);
		PipelineState::CreatePipeline("AreaEffect", AreaEffect, ADD);
		//PipelineState::CreatePipeline("PBShadowMap", PBShadowMap);
		//PipelineState::CreatePipeline("GrassShadowMap", GrassShadowMap);
		break;
	case 8:
		PipelineState::CreatePipeline("DrawShadowOBJ", DrawShadowOBJ);
		PipelineState::CreatePipeline("Blur", Blur);

		//PipelineState::CreatePipeline("Grass", GRASS);
		break;
	case 9:
		PipelineState::CreatePipeline("SSAO", Ssao);
		PipelineState::CreatePipeline("SSAOCombine", SsaoCombine);

		break; 
	case 10:
		PipelineState::CreatePipeline("FisheyeLens", FisheyeLens);
		PipelineState::CreatePipeline("NoAlphaToCoverageSprite", NoAlphaToCoverageSprite, ADD);

		
		break;
	case 11:
		PipelineState::CreatePipeline("NoAlphaToCoverageSprite", NoAlphaToCoverageSprite);
		PipelineState::CreatePipeline("Migrate", Migrate);

		
		break;
	//case 12:
	//	break;
	default:
		createPipelineFinish = true;

		break;
	}

	createPipelineLevel++;
}

void Game::LoadFinish()
{
	//DirectInputオブジェクトの生成
	Input::Initialize(win->GetHwnd());

	CollisionManager::GetInstance()->Initialize(Vector3{ -70.0f,-10.0f,-70.0f }+100, Vector3{ 70.0f,50.0f,70.0f }+100);
	

	lightCamera = std::make_unique<LightCamera>();
	lightCamera->SetDistance(100);
	lightCamera->SetLightDir({ dir[0],dir[1],dir[2] });
	Object3D::SetLightCamera(lightCamera.get());
	sceneManeger = SceneManager::GetInstance();
	sceneManeger->Initialize();
	sceneManeger->Add(Scene::SCENE::Title, new Title());
	sceneManeger->Add(Scene::SCENE::Play, new Play());
	//sceneManeger->Add(Scene::SCENE::Ending, new Ending());
	sceneManeger->Change(Scene::SCENE::Play);
	
	shadowMap.reset(new TextureResource("shadowMap",{1920,1080}, DXGI_FORMAT_R32_FLOAT,{0,0,0,0}));
	nowLoading = false;
}

void Game::DrawLoadTex()
{
	loadTex->DrawSprite("LoadPicture", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 });
	directX->DepthClear();
	loadDot->SpriteSetTextureRect("LoadDot", 0, 0, 42.0f * (createPipelineLevel % 8), 25);
	loadDot->DrawSprite("LoadDot", { 1560,1010 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 });
}

void Game::Initialize()
{
	win->CreateWidow(Window::WINDOW);

	// DirectX 初期化処理 ここから
	directX->Initialize(win.get());
	computeWrapper->Initialize();

	// DirectX 初期化処理 ここまで
	FPS::SetWindow(win.get());
	FbxLoader::GetInstance()->Initialize();
	Sprite::StaticInitialize(win.get());
	Audio::Initialize();
	//ShowCursor(false);
	nowLoading = true;

	ComputeShade::StaticInitialize();
	
	FPS::Initialize();
	
#ifdef _DEBUG
	DebugText::Initialize();
#endif // _DEBUG

	loadTex = std::make_unique<Sprite>();
	loadDot = std::make_unique<Sprite>();

}

void Game::Run()
{
	MSG msg{}; // メッセージ
	while (true)
	{

		FPS::StartMeasure();
		// メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージャにメッセージを送る
		}

		// 終了メッセージが来たらループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}

		if (nowLoading)
		{
			//アセットロード
			RoadAsset();
			//パイプラインの生成
			CreatePipeline();
			if (createPipelineFinish && loadAssetFinish)
				LoadFinish();
			directX->BeginDraw();
			directX->ImguiDraw();
			DrawLoadTex();

		}

		else if (!nowLoading)
		{
			Object::SetBbIndex();
			TextureResource::SetBbIndex();
			Sprite::SetBbIndex();
			Input::Update();
			Alpha::Update();
			if (Input::TriggerKey(DIK_1))
			{
				DrawMode::SetMode(DrawMode::None);
			}
			//else if (Input::TriggerKey(DIK_2))
			//{
			//	DrawMode::SetMode(DrawMode::Bloom);
			//}
			if(Input::TriggerKey(DIK_3))
			{
				DrawMode::SetDrawImGui(1 - DrawMode::GetDrawImGui());
			}
			//lightCamera->SetDistance(distance);
			//lightCamera->SetLightDir({ dir[0],dir[1],dir[2] });
			lightCamera->Update();
			Object3D::ClucLightViewProjection();
			sceneManeger->Update();
			ParticleEmitter::Update();
			directX->ComputeBegin();
			//2.画面クリアコマンドここまで
			Object3D::SetDrawShadow(true);
			shadowMap->PreDraw();
			directX->ImguiDraw();
			sceneManeger->PreDraw();
			//directX->DepthClear();
			shadowMap->PostDraw(false);
			Object3D::SetDrawShadow(false);

			//3.描画コマンドここから
			directX->BeginDraw();
			sceneManeger->PreDraw();
			CollisionManager::GetInstance()->DrawCollider();
			directX->DepthClear();
			//背面描画ここまで
#ifdef _DEBUG
			DebugText::Draw();
#endif // _DEBUG
			//directX->DepthClear();

			sceneManeger->PostDraw();
			ParticleEmitter::Draw();
		}
		directX->EndDraw();

		//ComputeWrapper::GetInstance()->MoveToNextFrame();
		LevelEditor::GetInstance()->Delete();
		FPS::FPSFixed();

	}

}

void Game::End()
{
	while (1)
	{
		if (ShowCursor(true) >= 0)
			break;
	}
	directX->End();
	computeWrapper->End();
	ParticleEmitter::End();
#ifdef _DEBUG
	DebugText::End();
#endif // _DEBUG
	FbxLoader::GetInstance()->Finalize();
	FBXManager::DeleteModels();
	//デリートはここまでに終わらせる
	//ComputeWrapper::GetInstance()->End();
	win->End();
}
