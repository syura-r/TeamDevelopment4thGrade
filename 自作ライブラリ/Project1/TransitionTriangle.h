#pragma once
#include "TransitionOneLine.h"

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

	static const int linesCount = 3;
	TransitionOneLine* lines[linesCount];

	//スタートのタイミングを測るタイマー
	int timer;
	const int gapTime = 15;//どのくらい遅らせるか
};

