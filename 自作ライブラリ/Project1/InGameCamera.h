#pragma once
#include "Camera.h"
#include "Input.h"
#include "ActorManager.h"
class InGameCamera :
    public Camera
{
public:

    InGameCamera();
	void Initialize();
	void Update();
	void RotateYaxis(Vector2 arg_inputVec);
	void AutoFocus(ActorManager* arg_actorManager);



	void SetDistance(float distance)
	{
		this->distance = distance;
		viewDirty = true;
	}


	void AddPhi(const float angle)
	{
		phi += angle;
		if (phi > 360 * XM_PI / 180.0f)
			phi -= 360 * XM_PI / 180.0f;
		else if (phi < 0)
			phi += 360 * XM_PI / 180.0f;
		viewDirty = true;
	}
	const float GetPhi()
	{
		return phi;
	}
	void SetPhi(const float arg_phi)
	{
		phi = arg_phi;
		if (phi > 360 * XM_PI / 180.0f)
			phi -= 360 * XM_PI / 180.0f;
		else if (phi < 0)
			phi += 360 * XM_PI / 180.0f;
		viewDirty = true;
	}

	void SetTheta(const float arg_theta)
	{
		theta = arg_theta;
		if (theta > 40 * XM_PI / 180.0f)
			theta = 40 * XM_PI / 180.0f;
		else if (theta < -40 * XM_PI / 180.0f)
			theta = -40 * XM_PI / 180.0f;
		viewDirty = true;
	}
private:
	//�����_�܂ŋ���
	float distance = 10;

	bool dirty;
};

