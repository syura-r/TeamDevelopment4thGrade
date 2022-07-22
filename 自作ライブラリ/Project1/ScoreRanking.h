#pragma once
#include "Sprite.h"
#include "NumberSprite.h"
#include "Object3D.h"

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
	//‰¤Š¥‚ð‚Ç‚ê‚É•t‚¯‚é‚©
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

	//‰¤Š¥
	struct Crown
	{
		Object3D* obj;
		Vector3 pos = { 0.0f,0.0f,0.0f };
		Vector3 scale = { 2,2,2 };
		Vector3 rotation = { 0,0,0 };
		Vector4 color = { 1,1,1,1 };

		bool isDisplay = false;
	};
	Crown* crowns[3];
};

