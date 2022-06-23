#pragma once
class ScoreManager
{
public:
	static ScoreManager* GetInstance();
	void Finalize();

	void Inisitlize();

	void AddScore_CutPanel(const int oncePanelNum);
	void AddScore_FallEnemy();
	const int GetTotalScore() { return totalScore; }

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
};

