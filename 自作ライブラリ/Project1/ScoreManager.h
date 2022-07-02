#pragma once
class ScoreManager
{
public:
	static ScoreManager* GetInstance();
	void Finalize();

	void Inisitlize();

	//��x�ɐ؂蔲�����p�l���ɂ����_
	void AddScore_CutPanel(const int oncePanelNum);
	//�G�𗎂Ƃ������Ƃɂ����_
	void AddScore_FallEnemy();

	const int GetTotalScore() { return totalScore; }
	const int GetCutPanelNum_All() { return cutPanelNum_all; }
	const int GetFallEnemyNum() { return fallEnemyNum; }

private:
	ScoreManager();
	~ScoreManager();

	static ScoreManager* sManager;

	//�X�R�A
	int totalScore = 0;
	//�p�l���؂蔲�����i�̂א��j
	int cutPanelNum_all = 0;
	//���Đ�
	int fallEnemyNum = 0;
};

