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
#include "TestBoss.h"
#include "Field.h"

#include "StandardEnemy.h"

Play::Play()
{
	next = Ending;
	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());
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
	menu = std::make_unique<Menu>();
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();
	actorManager = ActorManager::GetInstance();
	actorManager->Initialize();

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

	//for (int i = 0; i < 9; i++)
	//{
	//	lightGroup->SetPointLightActive(i, true);
	//	lightGroup->SetPointLightPos(i, lightPos[i].data() - Vector3{ 0,3,0 });
	//}

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
	objectManager->AddObjectsAtOnce();

	pause = new Pause();
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
	delete pause;
}

void Play::Initialize()
{
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	objectManager->Reset();
	actorManager->Initialize();

	//ゲームオブジェクト生成
	Field* field = new Field();
	objectManager->Add(field);	

	Player* player = new Player();
	objectManager->Add(player);	

	StandardEnemy* testEnemy = new StandardEnemy({ 0,-5, -10 }, 10);
	objectManager->Add(testEnemy);	

	isEnd = false;
	pause->Initialize();
}

void Play::Update()
{
	//ポーズのオンオフ
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		pause->SetUsePause(!pause->GetUsePause());
	}
	pause->Update();
	//やり直す
	if (pause->GetRestart())
	{
		Initialize();
	}
	//タイトルにもどる
	if (pause->GetToTitle())
	{
		next = Title;
		ShutDown();
	}
	if (pause->GetUsePause())
		return;

	camera->Update();

	if (Input::TriggerKey(DIK_7))
	{
		ParticleEmitter::CreateExplosion(Vector3(0, 0, 0));
	}

	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	objectManager->Update();
	collisionManager->CheckAllCollisions();
	if (!actorManager->GetPlayer()->GetReset())
		TimeUpdate();
}

void Play::PreDraw()
{
	pause->Draw();

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
		objectManager->PreDraw();
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;
		objectManager->PostDraw();
	if ( !Object3D::GetDrawShadow())
	{
		DirectXLib::GetInstance()->DepthClear();
	}

}

void Play::TimeUpdate()
{
}
