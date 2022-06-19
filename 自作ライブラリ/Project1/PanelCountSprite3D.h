#pragma once
#include "Sprite3D.h"

class PanelCountSprite3D
{
public:
	PanelCountSprite3D(Vector3& parentPosition, const std::string& parentName, int& panelNum);
	~PanelCountSprite3D();
	void Initialize();
	void Update();
	void Draw();

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

	//表示
	Sprite3D* icon_sp3D = nullptr;
	Sprite3D* num_sp3D[digit];

	std::string iconTexName;

	Vector3 position;

};
