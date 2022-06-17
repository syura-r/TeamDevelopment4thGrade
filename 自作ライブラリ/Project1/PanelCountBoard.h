#pragma once
#include "Object.h"

class PanelCountBoard :
	public Object
{
public:
	PanelCountBoard(Object* parentObject);
	~PanelCountBoard();
	void Initialize()override;
	void Update()override;

private:
	Object* parentObject = nullptr;

	//�擾��
	int panelNum;
};

