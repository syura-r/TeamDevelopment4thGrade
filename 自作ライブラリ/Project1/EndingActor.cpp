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

	changePanelNum = 0;
	changeTargetPanelNum = 0;
	isUpDownNext = true;

	upDownIntervalTimer = 0;

	isAddPanelEnd = false;

	ResetPanel();
}

void EndingActor::Update()
{
	//数値ロール
	if (isNumberRoll)
	{
		numberRoll_interval++;
		const int intervalLimit = 3;//何フレームごとにランダムな数値を取得するか
		if (numberRoll_interval > intervalLimit)
		{
			numberRoll_interval = 0;
			score_draw = (int)rand() % 99;
		}
	}
	else
	{
		//実際のスコアを表示
		score_draw = score;
	}

	//パネルの増減
	if (!isAddPanelEnd)
	{
		UpDownPanel();
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

int EndingActor::ChangeTargetPanelNum()
{
	//増減の範囲
	const int maxNum = 30;
	const int minNum = 5;

	//目標値を決める
	int resultNum = ((int)rand() % (maxNum - minNum)) + minNum;
	//ドラムロールのおわりでスコアの値に
	if (!isNumberRoll)
	{
		resultNum = score;
	}

	//目標値を返す
	return resultNum;
}

void EndingActor::UpDownPanel()
{
	//次の変化を決める
	if (isUpDownNext)
	{
		//目標値の設定
		changeTargetPanelNum = ChangeTargetPanelNum();
		//変化量の設定
		changePanelNum = changeTargetPanelNum - panels.size();
		isUpDownNext = false;
	}

	//増えるのか、減るのか
	bool isUp = false;
	bool isDown = false;
	int stockNum = changePanelNum;
	if (changePanelNum > 0)
	{
		isUp = true;
	}
	else if (changePanelNum < 0)
	{
		isDown = true;
		stockNum *= -1;//正の値に
	}

	//実際にパネルの増減をさせる
	const int intervalLimit = 3;
	if (upDownIntervalTimer >= intervalLimit)
	{
		if (isUp)
		{
			AddPanel();
		}
		else if (isDown)
		{
			SubPanel();
		}
		upDownIntervalTimer = 0;
	}
	upDownIntervalTimer++;

	//ドラムロールのおわりか、目標値に達したら次の目標値を決めるように
	if (!isNumberRoll || changeTargetPanelNum == panels.size())
	{
		isUpDownNext = true;
	}

	//ドラムロールのおわりかつ、目標値に達したらおわり
	if (!isNumberRoll && changeTargetPanelNum == panels.size())
	{
		isAddPanelEnd = true;
	}
}

void EndingActor::AddPanel()
{
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

	//Actorの移動
	position.y = pos_y_add * (panels.size() + 1);
}

void EndingActor::SubPanel()
{
	//削除
	delete panels.back()->object;
	delete panels.back();
	panels.pop_back();

	//Actorの移動
	float pos_y_add = 0.2f;
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
