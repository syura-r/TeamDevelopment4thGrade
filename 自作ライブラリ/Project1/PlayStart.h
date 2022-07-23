#pragma once
#include "Sprite.h"

class InGameCamera;
class PlayStart
{
public:
	PlayStart(InGameCamera* arg_camera);
	~PlayStart();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsActive() { return isActive; }
	void SetIsActive(const bool arg_isActive) { isActive = arg_isActive; }

private:
	bool isActive = false;

	//カメラ
	InGameCamera* camera;//コピー用
	void CameraZoomOut();
	bool isCameraZoomEnd = false;
	int easingCount_camera = 0;
	float cameraDistance = 0;
	const float cameraDistance_init = 25.0f;

	//レディー
	void ScalingReadySprite();
	Sprite* sp_ready = nullptr;
	bool isReadyEnd = false;
	int easingCount_ready = 0;
	float scale_ready = 0.0f;
	const float scale_ready_init = 0.0f;

	//ゴー
	void TranslationGoSprite();
	Sprite* sp_go = nullptr;
	bool isGoEnd = false;
	int easingCount_go = 0;
	float positionX_go = 0.0f;
	const float positionX_go_init = 1920.0f + 256.0f;

};

