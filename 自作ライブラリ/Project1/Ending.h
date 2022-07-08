#pragma once
#include "Scene.h"
#include"Texture.h"
#include"Alpha.h"
#include "NumberSprite.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include"LightGroup.h"

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
	static int killCount_player;
	static int killCount_enemyA;
	static int killCount_enemyB;

private:
	std::unique_ptr<DebugCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	void SelectMenu();

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;

	//
	struct ResultSet
	{
		ResultSet(const bool arg_isPlayer);
		~ResultSet();
		void Initialize(const int arg_killCount, const float arg_positionX_3d, const float arg_positionX_2d);
		void Update(const int arg_topKillCount);
		void Draw();

		//機体モデル関連
		Object3D* object = nullptr;
		Vector3 position = { 0.0f, -7.0f, 10.0f };
		Vector3 scale = { 1,1,1 };
		Vector3 rotation = { 0,180,0 };
		Vector4 color = { 1,1,1,1 };
		//王冠
		Object3D* obj_crown;
		Vector3 pos_crown = { position.x, 8.0f, position.z };
		Vector3 scale_crown = { 1,1,1 };
		Vector3 rotation_crown = { -15,0,0 };
		Vector4 color_crown = { 1,1,1,1 };
		bool isTop = false;//撃墜数が一番多いか
		bool drawCrown = false;//撃墜数が一番多いか
		//撃墜数
		NumberSprite* numberSprite = nullptr;
		//ゲージ
		Sprite* sp_gauge = nullptr;
		float scaleY_gauge = 0.0f;
		float scaleY_gauge_draw = 0.0f;
		float killCount = 0.0f;
		float killCount_draw = 0.0f;
		float positionX_2d;

		bool isPlayer = false;
	};
	static const int enemyCount = 2;//敵の数
	ResultSet* set[enemyCount + 1];

	//王冠をどれに付けるか
	void TopSearch();
	//ゲージのサイズを指定
	void GaugeSize();

	const float positions_3d[enemyCount + 1] = { -9.7f,0.0f,9.7f };
	const float positions_2d[enemyCount + 1] = { 500,960,1420 };

	//数字を増やす時用
	int topKillCount = 0;

	//選択項目
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};

	//リスタート
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 640, 950 };

	//タイトル
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 1280, 950 };

};

