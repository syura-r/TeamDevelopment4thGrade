#pragma once
#include "Object3D.h"
#include "NumberSprite.h"
#include "Crown.h"

class EndingActor
{
public:
	EndingActor(const std::string& arg_modelName);
	~EndingActor();

	void Initialize(const int arg_score, const int arg_ranking);
	void Update();
	void Draw();

	void SetIsNumberRoll(const bool arg_isNumberRoll) { isNumberRoll = arg_isNumberRoll; }

	bool GetIsAddPanelEnd() { return isAddPanelEnd; }
	int GetRanking() { return ranking; }
	Vector3& GetPosition() { return position; }

private:
	int score = 0;
	float score_draw = 0.0f;

	int ranking = 0;

	Object3D* object = nullptr;
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 scale = { 0.9f, 0.9f, 0.9f };
	Vector3 rotation = { 0,180,0 };
	Vector4 color = { 1,1,1,1 };

	Crown* crown = nullptr;

	//スコア数値
	NumberSprite* numberSprite = nullptr;
	Vector2 position_2d = { 0.0f, 0.0f };
	int numberRoll_interval = 0;
	bool isNumberRoll = false;

	//パネル
	struct Panel
	{
		Object3D* object = nullptr;
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 4.3f, 1.0f, 4.3f };
		Vector3 rotation = { 0,0,0 };
		Vector4 color = { 0.25f,0.58f,1,1 };
	};
	std::vector<Panel*> panels;
	void AddPanel();
	void ResetPanel();
	bool isAddPanelEnd = false;

};

