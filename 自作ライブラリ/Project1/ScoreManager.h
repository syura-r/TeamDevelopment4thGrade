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
	//スコア
	const int GetTotalScore() { return totalScore; }

	//ゲーム終了時のパネル所持数
	void SetStockPanelNum_Last(const int stockPanelNum) { this->stockPanelNum_last = stockPanelNum; }
	const int GetStockPanelNum_Last() { return stockPanelNum_last; }

private:
	ScoreManager();
	~ScoreManager();

	static ScoreManager* sManager;

	//スコア
	int totalScore = 0;
	//パネル所持数（最終）
	int stockPanelNum_last = 0;
	//撃墜数
	int fallEnemyNum = 0;
};

