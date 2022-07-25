#include "TransitionTriangle.h"

TransitionTriangle::TransitionTriangle()
{
	for (int i = 0; i < linesCount; i++)
	{
		lines[i] = new TransitionOneLine(linesCount, i);
	}
}

TransitionTriangle::~TransitionTriangle()
{
	for (int i = 0; i < linesCount; i++)
	{
		delete lines[i];
	}
}

void TransitionTriangle::Initialize()
{
	isOpen = false;
	isClose = false;
	timer = 0;

	for (int i = 0; i < linesCount; i++)
	{
		lines[i]->Initialize();
	}
}

void TransitionTriangle::Update()
{
	if (!isOpen && !isClose)
		return;

	bool isLinesEnd = true;//ìÆÇ´èIÇÌÇ¡ÇΩÇ©
	for (int i = 0; i < (timer / gapTime) + 1; i++)
	{
		if (i >=linesCount)
		{
			continue;
		}

		lines[i]->Update();

		isLinesEnd = isLinesEnd && !lines[i]->GetIsActive();
	}

	//èIóπ
	if (isLinesEnd)
	{
		isOpen = false;
		isClose = false;
	}

	timer++;
}

void TransitionTriangle::Draw()
{
	for (int i = 0; i < linesCount; i++)
	{
		lines[i]->Draw();
	}
}

void TransitionTriangle::IsOpen()
{
	if (isOpen)
	{
		return;
	}

	isOpen = true;
	isClose = false;
	timer = 0;

	for (int i = 0; i < linesCount; i++)
	{
		lines[i]->StartTransition(TransitionOneLine::Transition::OPEN);
	}
}

void TransitionTriangle::IsClose()
{
	if (isClose)
	{
		return;
	}

	isClose = true;
	isOpen = false;
	timer = 0;

	for (int i = 0; i < linesCount; i++)
	{
		lines[i]->StartTransition(TransitionOneLine::Transition::CLOSE);
	}
}
