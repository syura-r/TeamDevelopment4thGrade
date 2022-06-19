#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "NumberSprite.h"

class Ending :public Scene
{
public:
	Ending();
	~Ending()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;

private:
	void SelectMenu();

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;

	//���U���g
	Sprite* sp_result = nullptr;

	//�݌v�X�R�A
	Sprite* sp_score = nullptr;
	NumberSprite* numSp_score = nullptr;
	float score = 0.0f;
	float drawScore = 0.0f;
	bool isCountEnd_score = false;

	//�����p�l�����i�ŏI�j
	Sprite* sp_panel = nullptr;
	NumberSprite* numSp_panel = nullptr;
	float panelNum = 0.0f;
	float drawPanelNum = 0.0f;
	bool isCountEnd_panel = false;

	//�I������
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};

	//���X�^�[�g
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 640, 864 };

	//�^�C�g��
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 1280, 864 };

};

