#pragma once
#include "Sprite.h"

class PlayEnd
{
public:
	PlayEnd();
	~PlayEnd();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsActive() { return isActive; }
	bool GetIsFinishEnd() { return isFinishEnd; }
	void SetIsActive(const bool arg_isActive) { isActive = arg_isActive; }

private:
	bool isActive = false;

	//フィニッシュ
	void TranslationFinishSprite();
	Sprite* sp_finish = nullptr;
	bool isFinishEnd = false;
	int easingCount_finish = 0;
	float positionX_finish = 0.0f;
	const float positionX_finish_init = 1920.0f + 256.0f;

};

