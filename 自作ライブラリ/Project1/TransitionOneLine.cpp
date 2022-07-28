#include "TransitionOneLine.h"
#include "Easing.h"

TransitionOneLine::TransitionOneLine(const int arg_linesCount_all, const int arg_linesCount)
{
	linesCount_all = arg_linesCount_all;
	linesCount = arg_linesCount;
	triangle = new Sprite();
	square = new Sprite();
}

TransitionOneLine::~TransitionOneLine()
{
	delete triangle;
	delete square;
}

void TransitionOneLine::Initialize()
{
	isActive = false;
	transition = Transition::CLOSE;
	timer = 0;
	texName_tri = "Fade_Tri";
	position_triangle = {
		positon_triangle_X_start,
		1080.0f / (linesCount_all * 2) * (2 * linesCount + 1),
	};

	scale_triangle = { (1080.0f / 850.0f) / linesCount_all, (1080.0f / 850.0f) / linesCount_all };
	scale_square = { 0.0f, 1080.0f / linesCount_all };
	anchar_square = { 0.0f, 0.5f };
}

void TransitionOneLine::Update()
{
	if (!isActive)
	{
		return;
	}

	//¶‚©‚ç‰E‚ÖˆÚ“®
	position_triangle.x = Easing::EaseOutQuart(
		positon_triangle_X_start, positon_triangle_X_end,
		limitTime, timer);

	//ˆø‚«L‚Î‚µ
	if (transition == Transition::CLOSE)
	{
		scale_square.x = position_triangle.x;
		anchar_square.x = 1.0f;
	}
	else
	{
		scale_square.x = 1920.0f - position_triangle.x;
		anchar_square.x = 0.0f;
	}

	//I—¹
	if (timer >= limitTime)
	{
		isActive = false;
	}

	timer++;
}

void TransitionOneLine::Draw()
{
	triangle->DrawSprite(texName_tri, position_triangle, 0.0f, scale_triangle);
	square->DrawSprite("white1x1", position_triangle, 0.0f, scale_square, { 0,0,0,1 }, anchar_square);
}

void TransitionOneLine::StartTransition(const Transition arg_transition)
{
	isActive = true;
	transition = arg_transition;
	timer = 0;

	if ((int)rand() % 2)
	{
		texName_tri = "Fade_Tri";
	}
	else
	{
		texName_tri = "Fade_TriY";
	}
}
