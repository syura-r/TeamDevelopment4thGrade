#include "FeverUI.h"
#include "ActorManager.h"
#include "Field.h"
#include "Player.h"
#include "StandardEnemy.h"

FeverUI::FeverUI()
{
	sp = new Sprite();
	numsp = new NumberSprite(drawNum);
}

FeverUI::~FeverUI()
{
	delete sp;
	delete numsp;
}

void FeverUI::Initialize()
{
	drawNum = 0;
	isInFever = false;
	position = {};
	scale = { 1,1 };
	color = { 1,1,1,1 };
}

void FeverUI::Update()
{
	//
	ActorManager* aManager = ActorManager::GetInstance();

	//�摜�T�C�Y
	const Vector2 textTexSize = { 746.0f, 54.0f };
	//�t�B�[�o�[����
	isInFever = aManager->GetPlayer()->IsInFever() ||
		aManager->GetStandardEnemies()[0]->IsInFever() ||
		aManager->GetStandardEnemies()[1]->IsInFever();

	//�t�B�[�o�[���̂��낢��ύX
	if (isInFever)
	{
		texName = "GamePlay_UI_FeverTimeText";
		Vector2 spaceSize = { 0.0f, 60.0f };//�]��
		position = { 960 + spaceSize.x, (textTexSize.y / 2) + spaceSize.y };
		scale = { 1.3f,1.3f };
		//�F���ω�
		const float speed = 0.1f;
		if (color.x >= 1.0f && color.y >= 1.0f && color.z >= 1.0f)
		{
			//���i�����l�j�Ȃ�Ԃ�
			color = { 1.0f,0.0f,0.0f,1 };
		}
		else if (color.x >= 1.0f && color.z <= 0.0f
			&& color.y < 1.0f)
		{
			//�ԂȂ�΂𑝂₵�ĉ��F��
			color.y += speed;
		}
		else if (color.y >= 1.0f && color.z <= 0.0f
			&& color.x > 0.0f)
		{
			//���F�Ȃ�Ԃ����炵�ė΂�
			color.x -= speed;
		}
		else if (color.y >= 1.0f && color.x <= 0.0f
			&& color.z < 1.0f)
		{
			//�΂Ȃ�𑝂₵�Đ��F��
			color.z += speed;
		}
		else if (color.z >= 1.0f && color.x <= 0.0f
			&& color.y > 0.0f)
		{
			//���F�Ȃ�΂����炵�Đ�
			color.y -= speed;
		}
		else if (color.z >= 1.0f && color.y <= 0.0f
			&& color.x < 1.0f)
		{
			//�Ȃ�Ԃ𑝂₵�Ď���
			color.x += speed;
		}
		else if (color.x >= 1.0f && color.y <= 0.0f
			&& color.z > 0.0f)
		{
			//���Ȃ�����炵�ĐԂ�
			color.z -= speed;
		}
		return;
	}
	else//�t�B�[�o�[���ȊO
	{
		texName = "GamePlay_UI_FeverText";
		Vector2 spaceSize = { 25.0f, 30.0f };//�]��
		position = { (textTexSize.x / 2) + spaceSize.x, (textTexSize.y / 2) + spaceSize.y };
		scale = { 1.0f,1.0f };
		color = { 1.0f,1.0f,1.0f,1.0f };
	}

	//�����̕ϓ�
	Field* field = aManager->GetFields()[0];
	int cutPanelNum = field->GetGottenCount();
	while (cutPanelNum >= field->GetFeverNolma())
	{
		cutPanelNum -= field->GetFeverNolma();
	}
	const int toFeverNum = field->GetFeverNolma() - cutPanelNum;

	//���ɍ��킹�ĉ��Z
	if (drawNum < toFeverNum)
	{
		const int sub = toFeverNum - drawNum;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else
		{
			addNum = 100;
		}

		drawNum += addNum;
	}
	//���ɍ��킹�Č��Z
	else if (drawNum > toFeverNum)
	{
		const int sub = drawNum - toFeverNum;
		int addNum = 1;
		if (sub <= 10)
		{
			addNum = 1;
		}
		else if (sub <= 100)
		{
			addNum = 10;
		}
		else
		{
			addNum = 100;
		}

		drawNum -= addNum;
	}
}

void FeverUI::Draw()
{
	sp->DrawSprite(texName, position, 0.0f, scale, color);

	if (isInFever)
	{
		//�t�B�[�o�[���͐����̕`��͂��Ȃ�
		return;
	}

	const float numberSpaceAddSize = 85.0f;
	position.x += numberSpaceAddSize;//�����摜�̐����̔z�u����ʒu�܂�
	std::string nuwNum = std::to_string((int)drawNum);
	int digit = nuwNum.size();//0�Ŗ��߂Ȃ�����
	numsp->Draw(digit, "GamePlay_UI_Number", position);
}
