#include "EndingActor.h"
#include "FBXManager.h"
#include "OBJLoader.h"

EndingActor::EndingActor(const std::string& arg_modelName)
{
	object = Object3D::Create(FBXManager::GetModel(arg_modelName), position, scale, rotation, color);
	crown = new Crown();
	numberSprite = new NumberSprite(score_draw);
}

EndingActor::~EndingActor()
{
	delete object;
	delete crown;
	delete numberSprite;
	ResetPanel();
}

void EndingActor::Initialize(const int arg_score, const int arg_ranking, const Vector4& arg_numberColor)
{
	score = arg_score;
	score_draw = 0.0f;
	ranking = arg_ranking;
	numberColor = arg_numberColor;

	//配置位置
	const float position_3dx[3] = { 0.0f, -9.7f, 9.7f };
	const float position_2dx[3] = { 960.0f, 650.0f, 1270.0f };
	position = { position_3dx[arg_ranking - 1], 0.0f, 0.0f };
	position_2d = { position_2dx[arg_ranking - 1], 200.0f };

	crown->Initialize(3.0f);

	numberRoll_interval = 0;
	isNumberRoll = true;

	ResetPanel();
	isAddPanelEnd = false;
}

void EndingActor::Update()
{
	AddPanel();

	//数値ロール
	if (isNumberRoll)
	{
		numberRoll_interval++;
		const int intervalLimit = 3;
		if (numberRoll_interval > intervalLimit)
		{
			numberRoll_interval = 0;
			score_draw = (int)rand() % 99;
		}
	}
	else
	{
		score_draw = score;
	}

	object->Update();
	crown->Update(ranking, position);
	for (auto& it : panels)
	{
		it->object->Update();
	}
}

void EndingActor::Draw()
{
	PipelineState::SetPipeline("FBX");
	object->Draw(true);

	PipelineState::SetPipeline("BasicObj");
	crown->Draw();
	for (auto& it : panels)
	{
		it->object->Draw();
	}

	PipelineState::SetPipeline("Sprite");
	std::string nuwNum = std::to_string((int)score_draw);
	int digit = nuwNum.size();//0で埋めないため
	if (digit <= 0)
		digit = 1;
	numberSprite->Draw(digit, "GamePlay_UI_Number", position_2d, { 1.0f,1.0f }, numberColor);
}

void EndingActor::AddPanel()
{
	if (score <= panels.size())
	{
		isAddPanelEnd = true;
		return;
	}

	Panel* newPanel = new Panel();
	newPanel->object = Object3D::Create(
		OBJLoader::GetModel("fieldPiece"),
		newPanel->position, newPanel->scale, newPanel->rotation, newPanel->color);

	//ズラして重ねる
	float pos_y_add = 0.2f;
	float pos_y = pos_y_add * panels.size();
	newPanel->position.x = position.x;
	newPanel->position.y += pos_y;
	newPanel->position.z = position.z;

	//追加
	panels.push_back(newPanel);

	//Actorもズラす
	position.y = pos_y_add * (panels.size() + 1);
}

void EndingActor::ResetPanel()
{
	for (auto& it : panels)
	{
		delete it->object;
	}
	for (auto it = panels.begin(); it != panels.end(); ++it)
	{
		delete* it;
	}
	panels.clear();
}
