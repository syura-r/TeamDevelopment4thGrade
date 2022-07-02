#pragma once
class ScoreManager
{
public:
	static ScoreManager* GetInstance();
	void Finalize();

	void Inisitlize();

	//一度に切り抜いたパネルによる加点
	void AddScore_CutPanel(const int oncePanelNum);
	//敵を落としたことによる加点
	void AddScore_FallEnemy();

	const int GetTotalScore() { return totalScore; }
	const int GetCutPanelNum_All() { return cutPanelNum_all; }
	const int GetFallEnemyNum() { return fallEnemyNum; }

private:
	ScoreManager();
	~ScoreManager();

	static ScoreManager* sManager;

	//スコア
	int totalScore = 0;
	//パネル切り抜き数（のべ数）
	int cutPanelNum_all = 0;
	//撃墜数
	int fallEnemyNum = 0;
};

