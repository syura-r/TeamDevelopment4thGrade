#pragma once
#include "Camera.h"
class Object;
class ScreenCamera :
    public Camera
{
public:
    ScreenCamera();
	void SetTargetObj(Object* targetObj)
	{
		ScreenCamera::targetObj = targetObj;
	}
	void Update();
private:
	//�����_�܂ŋ���
	float distance = 30;
	Object* targetObj = nullptr;
};

