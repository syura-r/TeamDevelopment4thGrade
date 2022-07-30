#pragma once
#include "Sprite.h"
#include "NumberSprite.h"
#include "Crown.h"

class ScoreRanking
{
public:
	ScoreRanking();
	~ScoreRanking();

	void Initialize();
	void Update();
	void Draw_UI();
	void Draw_OBJ();


private:
	//王冠をどれに付けるか
	void TopSearch();

	void CheckScore();
	void CheckRank();
	void CheckPosition();

	struct ActorScoreUI
	{
		ActorScoreUI(const std::string& arg_texName);
		~ActorScoreUI();

		void Initialize();
		void Update();
		void Draw(const float arg_basePosition_x);

		Sprite* icon = nullptr;
		Sprite* cross = nullptr;
		NumberSprite* score_sp = nullptr;
		NumberSprite* rank_sp = nullptr;

		float score = 0.0f;
		Vector2 position = {};
		std::string texName = "";

		float rank = 0.0f;
	};

	Sprite* back = nullptr;
	Vector2 position_base = {};

	ActorScoreUI* blue_UI = nullptr;
	ActorScoreUI* red_UI = nullptr;
	ActorScoreUI* green_UI = nullptr;

	//王冠
	Crown* crowns[3];
};

