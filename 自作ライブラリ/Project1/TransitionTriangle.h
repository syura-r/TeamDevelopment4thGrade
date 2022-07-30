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

	//�X�^�[�g�̃^�C�~���O�𑪂�^�C�}�[
	int timer;
	const int gapTime = 15;//�ǂ̂��炢�x�点�邩
};

