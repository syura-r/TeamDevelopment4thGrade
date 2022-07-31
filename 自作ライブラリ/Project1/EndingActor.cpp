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

	//�z�u�ʒu
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
	//���l���[��
	if (isNumberRoll)
	{
		numberRoll_interval++;
		const int intervalLimit = 3;//���t���[�����ƂɃ����_���Ȑ��l���擾���邩
		if (numberRoll_interval > intervalLimit)
		{
			numberRoll_interval = 0;
			score_draw = (int)rand() % 99;
		}
	}
	else
	{
		//���ۂ̃X�R�A��\��
		score_draw = score;
	}

	//�p�l���̑���
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
	int digit = nuwNum.size();//0�Ŗ��߂Ȃ�����
	if (digit <= 0)
		digit = 1;
	numberSprite->Draw(digit, "GamePlay_UI_Number", position_2d, { 1.0f,1.0f }, numberColor);
}

int EndingActor::ChangeTargetPanelNum()
{
	//�����͈̔�
	const int maxNum = 30;
	const int minNum = 5;

	//�ڕW�l�����߂�
	int resultNum = ((int)rand() % (maxNum - minNum)) + minNum;
	//�h�������[���̂����ŃX�R�A�̒l��
	if (!isNumberRoll)
	{
		resultNum = score;
	}

	//�ڕW�l��Ԃ�
	return resultNum;
}

void EndingActor::UpDownPanel()
{
	//���̕ω������߂�
	if (isUpDownNext)
	{
		//�ڕW�l�̐ݒ�
		changeTargetPanelNum = ChangeTargetPanelNum();
		//�ω��ʂ̐ݒ�
		changePanelNum = changeTargetPanelNum - panels.size();
		isUpDownNext = false;
	}

	//������̂��A����̂�
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
		stockNum *= -1;//���̒l��
	}

	//���ۂɃp�l���̑�����������
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

	//�h�������[���̂���肩�A�ڕW�l�ɒB�����玟�̖ڕW�l�����߂�悤��
	if (!isNumberRoll || changeTargetPanelNum == panels.size())
	{
		isUpDownNext = true;
	}

	//�h�������[���̂���肩�A�ڕW�l�ɒB�����炨���
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

	//�Y�����ďd�˂�
	float pos_y_add = 0.2f;
	float pos_y = pos_y_add * panels.size();
	newPanel->position.x = position.x;
	newPanel->position.y += pos_y;
	newPanel->position.z = position.z;

	//�ǉ�
	panels.push_back(newPanel);

	//Actor�̈ړ�
	position.y = pos_y_add * (panels.size() + 1);
}

void EndingActor::SubPanel()
{
	//�폜
	delete panels.back()->object;
	delete panels.back();
	panels.pop_back();

	//Actor�̈ړ�
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
