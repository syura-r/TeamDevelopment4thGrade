#pragma once
#include "Sprite.h"
#include "Vector.h"

class TransitionTriangle
{
public:
	TransitionTriangle();
	~TransitionTriangle();

	void Initialize();
	void Update();
	void Draw();

	void IsOpen();
	void IsClose();

	bool GetIsOpen() { return isOpen; }
	bool GetIsClose() { return isClose; }

private:
	bool isOpen = false;
	bool isClose = false;

	Sprite* triangle = nullptr;
	Sprite* square = nullptr;

	Vector2 position_triangle;
	const Vector2 position_triangle_init = { -615, 1080 / 2 };
	const Vector2 position_triangle_end = { 1920 + (-position_triangle_init.x), 1080 / 2 };

	Vector2 anchar_square;
	Vector2 scale_square;
};

