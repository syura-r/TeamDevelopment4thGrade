#pragma once
#include "Sprite.h"
#include "Vector.h"

class TransitionOneLine
{
public:
	TransitionOneLine(const int arg_linesCount_all, const int arg_linesCount);
	~TransitionOneLine();

	void Initialize();
	void Update();
	void Draw();

	enum Transition
	{
		OPEN,
		CLOSE
	};

	void StartTransition(const Transition arg_transition);

	bool GetIsActive() { return isActive; }

private:
	int linesCount_all;//全体で何本か
	int linesCount;//全体の何本目か

	bool isActive;
	Transition transition;//開くか閉じるか
	int timer;//イージング用
	const int limitTime = 120;//

	Sprite* triangle = nullptr;
	Sprite* square = nullptr;

	Vector2 position_triangle;
	Vector2 scale_triangle;
	const float positon_triangle_X_start = -615.0f;
	const float positon_triangle_X_end = 1920.0f - positon_triangle_X_start;

	Vector2 anchar_square;
	Vector2 scale_square;
};

