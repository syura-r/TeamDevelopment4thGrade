#pragma once
#include "Sprite.h"
#include "Vector.h"

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
	Vector4 colorGauge = { 1,1,1,1 };

	int level = 0;
	int nextLevelCount = 0;//�������Ƀ��x�����オ�邩
	const int maxGaugeStep = 15;//�Q�[�W�̑��i��
	int gaugeCount = 0;

	//���x������
	Sprite* sp_levelText = nullptr;

	//���x�����ς������
	bool newlevel = false;
	int prev_level = 0;
	int timer_newLevelAction = 0;
	void NewLevelAction();
};

