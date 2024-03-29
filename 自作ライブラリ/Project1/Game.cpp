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

int SettingParam::padSensitive = 3;
int SettingParam::reverseX = 1;
int SettingParam::reverseY = 1;

int SettingParam::jumpButton = XINPUT_GAMEPAD_A;
int SettingParam::airSlideButton = XINPUT_GAMEPAD_B;
int SettingParam::runButton = XINPUT_GAMEPAD_LEFT_SHOULDER;
int SettingParam::resetButton = XINPUT_GAMEPAD_RIGHT_SHOULDER;
bool SettingParam::viewCollision = false;

bool SettingParam::onSSAO = false;

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
		Texture::LoadTexture("number", "number.png");
		Texture::LoadTexture("wind", "wind.png");
		Texture::LoadTexture("frame_B", "frame_B.png");
		Texture::LoadTexture("frame_X", "frame_X.png");
		Texture::LoadTexture("frame_Y", "frame_Y.png");
		Texture::LoadTexture("frame_next", "frame_next.png");
		Texture::LoadTexture("frame_blank", "frame_blank.png");
		Texture::LoadTexture("shape_star", "shape_star.png");
		Texture::LoadTexture("shape_triforce", "shape_triforce.png");
		Texture::LoadTexture("shape_ribbon", "shape_ribbon.png");
		Texture::LoadTexture("shape_triangle", "shape_triangle.png");
		Texture::LoadTexture("shape_pentagon", "shape_pentagon.png");
		Texture::LoadTexture("shape_hexagram", "shape_hexagram.png");
		Texture::LoadTexture("s_LIFE", "s_LIFE.png");
		Texture::LoadTexture("s_LBorRB", "s_LBorRB.png");
		Texture::LoadTexture("s_GameOver", "s_GameOver.png");
		Texture::LoadTexture("s_Fever", "s_Fever.png");
		Texture::LoadTexture("titlelogo", "Title/titlelogo.png");
		Texture::LoadTexture("titlestart", "Title/titlestart.png");
		Texture::LoadTexture("toGame", "Pause/toGame.png");
		Texture::LoadTexture("restart", "Pause/restart.png");
		Texture::LoadTexture("toTitle", "Pause/toTitle.png");
		Texture::LoadTexture("GamePlay_UV_Player", "GamePlay_UV_Player.png");
		Texture::LoadTexture("GamePlay_UI_Number", "GamePlay_UI_Number.png");
		Texture::LoadTexture("GamePlay_UI_Colon", "GamePlay_UI_Colon.png");
		Texture::LoadTexture("GamePlay_UI_Gettriangle_Player", "GamePlay_UI_Gettriangle_Player.png");
		Texture::LoadTexture("GamePlay_UI_Gettriangle_Enemy", "GamePlay_UI_Gettriangle_Enemy.png");
		Texture::LoadTexture("GamePlay_UI_Gettriangle_Enemy2", "GamePlay_UI_Gettriangle_Enemy2.png");
		Texture::LoadTexture("Result_UI_Gettriangle_text", "Result/Result_UI_Gettriangle_text.png");
		Texture::LoadTexture("Result_UI_Totalscore_Text", "Result/Result_UI_Totalscore_Text.png");
		Texture::LoadTexture("GamePlay_UI_Score_Text", "GamePlay_UI_Score_Text.png");
		Texture::LoadTexture("Result_UI_Drop_text", "Result/Result_UI_Drop_text.png");
		Texture::LoadTexture("GamePlay_UI_TimeLimit_30", "TimeLimit/GamePlay_UI_TimeLimit_30.png");
		Texture::LoadTexture("GamePlay_UI_TimeLimit_60", "TimeLimit/GamePlay_UI_TimeLimit_60.png");
		Texture::LoadTexture("GamePlay_UI_TimeLimit_90", "TimeLimit/GamePlay_UI_TimeLimit_90.png");
		//Texture::LoadTexture("GamePlay_UI_TimeLimit_120", "TimeLimit/GamePlay_UI_TimeLimit_120.png");
		//Texture::LoadTexture("GamePlay_UI_TimeLimit_150", "TimeLimit/GamePlay_UI_TimeLimit_150.png");
		Texture::LoadTexture("sound", "Pause/sound.png");
		Texture::LoadTexture("BGM", "Pause/BGM.png");
		Texture::LoadTexture("SE", "Pause/SE.png");
		Texture::LoadTexture("circle", "Pause/circle.png");
		Texture::LoadTexture("GamePlay_UI_Operation", "Pause/GamePlay_UI_Operation.png");
		Texture::LoadTexture("FeverCuttingEffect", "GamePLay_FeverEffect.png");
		Texture::LoadTexture("FeverPlayerEffect", "GamePlay_SPeffect.png");
		Texture::LoadTexture("GamePlay_UI_FeverText", "GamePlay_UI_FeverText.png");
		Texture::LoadTexture("GamePlay_UI_Playerpoint", "GamePlay_UI_Playerpoint.png");
		Texture::LoadTexture("GamePlay_UI_Enemypoint", "GamePlay_UI_Enemypoint.png");
		Texture::LoadTexture("GamePlay_UI_Enemy2point", "GamePlay_UI_Enemy2point.png");
		//Texture::LoadTexture("Result_Player_Gauge", "Result_Player_Gauge.png");
		//Texture::LoadTexture("Result_Enemy_Gauge", "Result_Enemy_Gauge.png");
		//Texture::LoadTexture("Result_Enemy2_Gauge", "Result_Enemy2_Gauge.png");
		Texture::LoadTexture("Result_UV_Crown", "Result_UV_Crown.png");
		Texture::LoadTexture("GamePlay_UI_Level1", "LevelGauge/GamePlay_UI_Level1.png");
		Texture::LoadTexture("GamePlay_UI_Level2", "LevelGauge/GamePlay_UI_Level2.png");
		Texture::LoadTexture("GamePlay_UI_Level3", "LevelGauge/GamePlay_UI_Level3.png");
		Texture::LoadTexture("GamePlay_UI_Level4", "LevelGauge/GamePlay_UI_Level4.png");
		Texture::LoadTexture("GamePlay_UI_Level5", "LevelGauge/GamePlay_UI_Level5.png");
		Texture::LoadTexture("GamePlay_UI_Level6", "LevelGauge/GamePlay_UI_Level6.png");
		Texture::LoadTexture("GamePlay_UI_Levelback", "LevelGauge/GamePlay_UI_Levelback.png");
		//Texture::LoadTexture("GamePlay_UI_Leveledge", "LevelGauge/GamePlay_UI_Leveledge.png");
		Texture::LoadTexture("GamePlay_UI_Levelgauge", "LevelGauge/GamePlay_UI_Levelgauge.png");
		Texture::LoadTexture("GamePlay_UI_Leveltri_1", "LevelGauge/GamePlay_UI_Leveltri_1.png");
		Texture::LoadTexture("GamePlay_UI_Leveltri_2", "LevelGauge/GamePlay_UI_Leveltri_2.png");
		Texture::LoadTexture("GamePlay_UI_Leveltri_3", "LevelGauge/GamePlay_UI_Leveltri_3.png");
		Texture::LoadTexture("GamePlay_UI_Leveltri_4", "LevelGauge/GamePlay_UI_Leveltri_4.png");
		Texture::LoadTexture("GamePlay_UI_Leveltri_5", "LevelGauge/GamePlay_UI_Leveltri_5.png");
		Texture::LoadTexture("GamePlay_UI_Maker", "GamePlay_UI_Maker.png");
		Texture::LoadTexture("GamePlay_CircleCover", "GamePlay_CircleCover.png");
		Texture::LoadTexture("GamePlay_Score_Rank", "ScoreRanking/GamePlay_Score_Rank.png");
		Texture::LoadTexture("GamePlay_Score_Batu", "ScoreRanking/GamePlay_Score_Batu.png");
		Texture::LoadTexture("GamePlay_Score_Player", "ScoreRanking/GamePlay_Score_Player.png");
		Texture::LoadTexture("GamePlay_Score_Enemy", "ScoreRanking/GamePlay_Score_Enemy.png");
		Texture::LoadTexture("GamePlay_Score_Enemy2", "ScoreRanking/GamePlay_Score_Enemy2.png");
		Texture::LoadTexture("Fade_Tri", "Fade_Tri.png");
		Texture::LoadTexture("Fade_TriY", "Fade_TriY.png");
		Texture::LoadTexture("GamePlay_UI_Ready", "GamePlay_UI_Ready.png");
		Texture::LoadTexture("GamePlay_UI_GO", "GamePlay_UI_GO.png");
		Texture::LoadTexture("GamePlay_UI_Finish", "GamePlay_UI_Finish.png");
		Texture::LoadTexture("GamePlay_UI_FeverTimeText", "GamePlay_UI_FeverTimeText.png");

		Texture::LoadTexture("FeverPlayerEffectWhite", "GamePlay_SPeffect_White.png");
		Texture::LoadTexture("DeadEffect", "GamePlay_UV_DeadEffect.png");

		break; 
	case 2:
		//Objモデルの読み込み
		OBJLoader::LoadModelFile("box", "box.obj", false);
		OBJLoader::LoadModelFile("sphere", "sphere.obj", true);
		OBJLoader::LoadModelFile("obBox", "bottomOriginBox.obj", false);
		OBJLoader::LoadModelFile("plate", "plate.obj", false);		
		break;
	case 3:
		//Objモデルの読み込み

		OBJLoader::LoadModelFile("floor", "floor.obj", true);
		OBJLoader::LoadModelFile("Hexagon", "Hexagon.obj", false);
		OBJLoader::LoadModelFile("fieldEdge", "GamePlay_Edge.obj", false);
		OBJLoader::LoadModelFile("fieldPiece", "GamePlay_Triangle.obj", false);
		OBJLoader::LoadModelFile("fieldPiece_bonus", "GamePlay_BonusTriangle.obj", false);
		OBJLoader::LoadModelFile("stadium", "stadium.obj", false);
		OBJLoader::LoadModelFile("lava", "GamePlay_Magma.obj", false);
		OBJLoader::LoadModelFile("Saw", "gamePlay_Saw.obj", false);
		OBJLoader::LoadModelFile("Crown", "Result_Crown.obj", false);
		OBJLoader::LoadModelFile("Area", "Area.obj", true);
		OBJLoader::LoadModelFile("DeadEffect", "GamePlay_DeadEffect.obj", false);

		break;
	case 4:
		//FBXファイルの読み込み
		FBXManager::LoadModelFile("player", "player", false);
		FBXManager::LoadModelFile("enemy", "player", false);
		FBXManager::LoadModelFile("GamePlay_Player", "GamePlay_Player", false);
		FBXManager::LoadModelFile("GamePlay_Enemy", "GamePlay_Enemy", false);
		FBXManager::LoadModelFile("GamePlay_Enemy2", "GamePlay_Enemy2", false);

		//WAVファイルの読み込み
		Audio::LoadFile("BGM_Title", "BGM/Title_BGM.wav");
		Audio::LoadFile("BGM_Play", "BGM/GamePlay_BGM.wav");
		Audio::LoadFile("BGM_PlayLimit", "BGM/GamePlay_BGM_Limit30.wav");
		Audio::LoadFile("BGM_Result", "BGM/Result_BGM.wav");
		Audio::LoadFile("SE_Decision", "SE/Allscene_SE_Decision.wav");
		Audio::LoadFile("SE_Select", "SE/Allscene_SE_Select.wav");
		Audio::LoadFile("SE_Collision", "SE/GamePlay_SE_Collision.wav");
		Audio::LoadFile("SE_Dash", "SE/GamePlay_SE_Dash.wav");
		Audio::LoadFile("SE_Fall", "SE/GamePlay_SE_Fall.wav");
		Audio::LoadFile("SE_GetSaw", "SE/GamePlay_SE_Getsaw.wav");
		Audio::LoadFile("SE_GetTriangle", "SE/GamePlay_SE_GetTriangle.wav");
		Audio::LoadFile("SE_SawCutNow", "SE/GamePlay_SE_Sawcut_Now.wav");
		Audio::LoadFile("SE_SawCutOut", "SE/GamePlay_SE_Sawcut_Out.wav");
		Audio::LoadFile("SE_SteppingOn", "SE/GamePlay_SE_Steppingon.wav");
		Audio::LoadFile("SE_TriangleLost", "SE/GamePlay_SE_Trianglelost.wav");
		Audio::LoadFile("SE_InFever", "SE/GamePlay_InFever.wav");
		Audio::LoadFile("SE_FeverCut", "SE/GamePlay_FeverCut.wav");
		Audio::LoadFile("SE_GetItemL2", "SE/GamePlay_SE_GetItem_L2.wav");
		Audio::LoadFile("SE_GetItemL3", "SE/GamePlay_SE_GetItem_L3.wav");
		Audio::LoadFile("SE_Limit30", "SE/GamePlay_SE_Limit30.wav");
		Audio::LoadFile("SE_Cheers", "SE/GamePlay_SE_CheersVoice.wav");
		Audio::LoadFile("SE_Count10", "SE/GamePlay_SE_Count10.wav");
		Audio::LoadFile("SE_Finish", "SE/GamePlay_SE_Finish.wav");
		Audio::LoadFile("SE_SceneTransition", "SE/Allscene_SE_Transition.wav");
		Audio::LoadFile("SE_DrumRoll", "SE/Result_SE_DrumRoll.wav");
		Audio::LoadFile("SE_RollFinish", "SE/Result_SE_RollFinish.wav");

		break;
	case 5:
		//FBXアニメーションの登録
		FBXManager::AddAnimationList("player", "walk",0,60);
		FBXManager::AddAnimationList("player", "stand", 61, 61);
		FBXManager::AddAnimationList("player", "run", 62, 121);
		FBXManager::AddAnimationList("player", "jump", 122, 152);
		FBXManager::AddAnimationList("player", "secondJump", 153, 180);
		FBXManager::AddAnimationList("player", "airSlide", 181, 190);

		FBXManager::AddAnimationList("enemy", "walk", 0, 60);
		FBXManager::AddAnimationList("enemy", "stand", 61, 61);

		FBXManager::AddAnimationList("GamePlay_Player", "move", 0, 60);

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
		//PipelineState::CreatePipeline("DepthSprite", DepthSPRITE);

		break;
	case 1:
		PipelineState::CreatePipeline("FBX", FBX);
		PipelineState::CreatePipeline("BasicObj", BasicObj);

		break;
	case 2:
		PipelineState::CreatePipeline("Particle", PARTICLE, ADD);
		//PipelineState::CreatePipeline("FBXPolygonBreak", FBXPolygonBreak);
		break;
	case 3:
		PipelineState::CreatePipeline("NoShade", NoShade);

		PipelineState::CreatePipeline("UVScrolling", UVScrolling);
		PipelineState::CreatePipeline("AreaEffect", AREAEFFECT,ADD);


		break;
	case 4:
		PipelineState::CreatePipeline("CollisionBox", ViewCollisionBox);
		PipelineState::CreatePipeline("CollisionSphere", ViewCollisionSphere);
		break;
	case 5:
		PipelineState::CreatePipeline("PieChart", PIECHART);
		break;
	case 6:
		//PipelineState::CreatePipeline("FBXShadowMap", FBXShadowMap);
		break;
	case 7:
		//PipelineState::CreatePipeline("ShadowMap", NormalShadowMap);
		break;
	case 8:
		//PipelineState::CreatePipeline("DrawShadowOBJ", DrawShadowOBJ);
		break;
	case 9:
		break;
	case 10:
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

	CollisionManager::GetInstance()->Initialize(Vector3{ -370.0f,-310.0f,-370.0f }+100, Vector3{ 370.0f,350.0f,370.0f }+100);
	

	lightCamera = std::make_unique<LightCamera>();
	lightCamera->SetDistance(100);
	lightCamera->SetLightDir({ dir[0],dir[1],dir[2] });
	Object3D::SetLightCamera(lightCamera.get());
	sceneManeger = SceneManager::GetInstance();
	sceneManeger->Initialize();
	sceneManeger->Add(Scene::SCENE::Title, new Title());
	sceneManeger->Add(Scene::SCENE::Play, new Play());
	sceneManeger->Add(Scene::SCENE::Ending, new Ending());

	sceneManeger->Change(Scene::SCENE::Title);

	//shadowMap.reset(new TextureResource("shadowMap",{1920,1080}, DXGI_FORMAT_R32_FLOAT,{0,0,0,0}));

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
	
	ParticleEmitter::Initialize();
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
			ParticleManager::GetInstance()->Update();
			directX->ComputeBegin();
			//2.画面クリアコマンドここまで
			//Object3D::SetDrawShadow(true);
			//shadowMap->PreDraw();
			//directX->ImguiDraw();
			//sceneManeger->PreDraw();
			////directX->DepthClear();
			//shadowMap->PostDraw(false);
			Object3D::SetDrawShadow(false);

			//3.描画コマンドここから
			directX->BeginDraw();
			directX->ImguiDraw();

			sceneManeger->PreDraw();
			CollisionManager::GetInstance()->DrawCollider();
			directX->DepthClear();
			//背面描画ここまで
#ifdef _DEBUG
			DebugText::Draw();
#endif // _DEBUG
			//directX->DepthClear();

			sceneManeger->PostDraw();
			ParticleManager::GetInstance()->Draw();
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
	ParticleManager::GetInstance()->End();
#ifdef _DEBUG
	DebugText::End();
#endif // _DEBUG
	FbxLoader::GetInstance()->Finalize();
	FBXManager::DeleteModels();
	Audio::End();
	//デリートはここまでに終わらせる
	//ComputeWrapper::GetInstance()->End();
	win->End();
}
