#include "TransitionTriangle.h"

TransitionTriangle::TransitionTriangle()
{
	triangle = new Sprite();
	square = new Sprite();
}

TransitionTriangle::~TransitionTriangle()
{
	delete triangle;
	delete square;
}

void TransitionTriangle::Initialize()
{
	position_triangle = position_triangle_init;
	anchar_square = { 1.0f,0.5f };
	scale_square = { 1920.0f, 1080.0f };

	isOpen = false;
	isClose = false;
}

void TransitionTriangle::Update()
{
	if (!isOpen && !isClose)
		return;

	//ˆÚ“®
	const float speed = 50.0f;
	position_triangle.x += speed;

	//ŽlŠpˆø‚«L‚Î‚µ
	if (isOpen)
		scale_square.x = 1920.0f - position_triangle.x;
	if (isClose)
		scale_square.x = position_triangle.x;

	//I—¹ðŒ
	if (position_triangle.x > position_triangle_end.x)
	{
		isOpen = false;
		isClose = false;
	}
}

void TransitionTriangle::Draw()
{
	triangle->DrawSprite("Fade_Tri", position_triangle, 0.0f, { 1.28f,1.28f });
	square->DrawSprite("white1x1", position_triangle, 0.0f, scale_square, { 0,0,0,1 }, anchar_square);
}

void TransitionTriangle::IsOpen()
{
	isOpen = true;
	isClose = false;
	position_triangle = position_triangle_init;
	anchar_square.x = 0.0f;
	scale_square.x = 1920.0f - position_triangle.x;
}

void TransitionTriangle::IsClose()
{
	isClose = true;
	isOpen = false;
	position_triangle = position_triangle_init;
	anchar_square.x = 1.0f;
	scale_square.x = position_triangle.x;
}
