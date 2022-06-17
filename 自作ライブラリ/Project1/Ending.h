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

public:
	static void SetScore(const float panelNum);

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
	const Vector2 pos_spScore = { 700, 300 };
	NumberSprite* numSp_score = nullptr;
	const Vector2 pos_numScore = { pos_spScore.x + (640 / 2) + (47 / 2 * 11), pos_spScore.y };
	float score = 0.0f;
	float drawScore = 0.0f;
	bool isCountEnd_score = false;

	//�ŏI�擾�p�l����
	Sprite* sp_panel = nullptr;
	const Vector2 pos_spPanel = { pos_spScore.x, 600 };
	NumberSprite* numSp_panel = nullptr;
	const Vector2 pos_numPanel = { pos_numScore.x, pos_spPanel.y };
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


private:
	static float panelNum;

};

