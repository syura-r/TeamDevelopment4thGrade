#pragma once
#include "Sprite.h"

class LevelGauge
{
public:
	LevelGauge();
	~LevelGauge();

	void Initialize();
	void Update();
	void Draw();

private:
	//�����w�i
	Sprite* sp_back = nullptr;
	//�g
	Sprite* sp_frame = nullptr;
	//�Q�[�W�̐�
	Sprite* sp_gauge = nullptr;
	int level = 0;
	int panelNum = 0;
	//���x������
	Sprite* sp_levelText = nullptr;

};

