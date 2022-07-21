#pragma once
#include "Scene.h"
#include "InGameCamera.h"
#include"LightGroup.h"
#include "Sprite.h"
#include "EndingActor.h"
#include "Stadium.h"

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
	static int score_player;
	static int score_enemy_red;
	static int score_enemy_green;

private:
	std::unique_ptr<InGameCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	void SelectMenu();
	//点滅
	void FlashMenu();

	enum SelectState
	{
		ToTitle = 0,
		Restart = 1,
	};
	SelectState selectState;

	//選択項目
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};
	Vector2 scale_select = {};
	float alpha_select = 1.0f;
	bool isUP_alphaChange = false;

	const Vector2 scaleBig_select = { 1.5f,1.5f };
	const Vector2 scaleSmall_select = { 1.0f,1.0f };

	//タイトル
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 640, 970 };
	Vector2 scale_title = { 1.0f,1.0f };
	float alpha_title = 1.0f;

	//リスタート
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 1280, 970 };
	Vector2 scale_restart = { 1.0f,1.0f };
	float alpha_restart = 1.0f;


	//順位計算
	void RankingSearch();
	//各キャラクター
	static const int actorsCount = 3;
	EndingActor* actors[actorsCount];
	//0から2に順位を格納する配列（順位は1～3）
	int ranking[actorsCount];

	//床オブジェクト
	Object3D* basePanel_object = nullptr;
	Vector3 basePanel_position = { 0.0f, -0.3f, 0.0f };
	Vector3 basePanel_scale = { 30.0f, 1.0f, 30.0f };
	Vector3 basePanel_rotation = { 0,0,0 };
	Vector4 basePanel_color = { 0.25f, 0.58f, 1.0f, 1.0f };

	//背景ステージ
	Stadium* stadium = nullptr;
};

