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
	//スコア計算
	void ScoreCalculation();

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;

	//リザルト
	Sprite* sp_result = nullptr;

	//累計スコア
	Sprite* sp_score = nullptr;

	NumberSprite* numSp_score = nullptr;
	float score = 0.0f;
	float drawScore = 0.0f;
	bool isCountEnd_score = false;

	//最終取得パネル数
	Sprite* sp_panel = nullptr;
	NumberSprite* numSp_panel = nullptr;
	float drawPanelNum = 0.0f;
	bool isCountEnd_panel = false;

	//選択項目
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};

	//リスタート
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 640, 864 };

	//タイトル
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 1280, 864 };


private:
	static float panelNum;

};

