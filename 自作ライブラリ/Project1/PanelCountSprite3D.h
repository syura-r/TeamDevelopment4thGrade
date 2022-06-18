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
	//�e���̐��l�����߂�
	void DigitNum();

	//�e�̍��W
	Vector3& parentPosition;
	//������
	int& panelNum;
	//�������̊e���̐�
	static const int digit = 3;
	int drawNum[digit];

	//�\��
	Sprite3D* icon_sp3D = nullptr;
	Sprite3D* num_sp3D[digit];

	std::string iconTexName;

	Vector3 position;

};
