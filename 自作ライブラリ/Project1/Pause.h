#pragma once
#include "Sprite.h"
#include "Vector.h"

class Pause
{
public:
	Pause();
	~Pause();

	void Initialize();
	void Update();
	void Draw();

	bool GetActivePause() { return activeFlag; }

	bool GetToGame() { return flag_toGame; }
	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//選択肢を選ぶ処理
	void Select();
	//決定ボタンを押した後の処理
	void Decision();

	//描画や更新処理を行うか
	bool activeFlag = false;
	//
	static bool fadeFlag;


	enum SelectState
	{
		ToGame = 0,
		Restart = 1,
		ToTitle = 2,
		Sound = 3,
	};
	SelectState selectState;
	//選択肢の個数
	const int selectMax = 4;

	//X座標のずらす基準
	static const int positionStepMax = 3;
	static float positions_X[positionStepMax];

	//ゲーム画面を暗く
	Sprite* sp_back = nullptr;
	Vector2 pos_back = {};

	//選択を見やすく
	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};

	//選択肢1つに必要な変数
	struct SelectSprite
	{
		SelectSprite();
		~SelectSprite();
		void Initialize(const std::string& texName, const float posY);
		void Update();
		void Draw();

		//動き出す前準備
		void PreMoveSetting();

		Sprite* sprite = nullptr;
		std::string texName = "";
		Vector2 pos = {};
		Vector2 prevPos = {};
		int step = 0;
		int easeTime = 0;
	};


	//ゲームにもどる
	SelectSprite* toGame = nullptr;
	bool flag_toGame = false;

	//やり直す
	SelectSprite* restart = nullptr;
	bool flag_restart = false;

	//タイトルにもどる
	SelectSprite* toTitle = nullptr;
	bool flag_toTitle = false;

	//音量設定
	SelectSprite* sound = nullptr;
	bool flag_sound = false;//BGMとSEの設定を展開する

};
