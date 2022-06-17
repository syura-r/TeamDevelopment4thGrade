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
	//�e���̐��l�����߂�
	void DigitNum();


	//�e�̍��W
	Vector3& parentPosition;
	//������
	int& panelNum;
	//�������̊e���̐�
	static const int digit = 3;
	int drawNum[digit];
};

