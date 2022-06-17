#pragma once
#include "Object.h"

#include "Sprite.h"

class PanelCountBoard :
	public Object
{
public:
	PanelCountBoard(Vector3& parentPosition, const std::string& parentName, int& panelNum);
	~PanelCountBoard();
	void Initialize()override;
	void Update()override;

private:
	//各桁の数値を求める
	void DigitNum();


	//親の座標
	Vector3& parentPosition;
	//所持数
	int& panelNum;
	//所持数の各桁の数
	static const int digit = 3;
	int drawNum[digit];
};

