#include "PlayStart.h"
#include "Easing.h"
#include "ActorManager.h"
#include "Player.h"
#include "Field.h"

PlayStart::PlayStart(InGameCamera* arg_camera)
{
	camera = arg_camera;
	sp_ready = new Sprite();
	sp_go = new Sprite();
}

PlayStart::~PlayStart()
{
	delete sp_ready;
	delete sp_go;
}

void PlayStart::Initialize()
{
	isActive = false;

	isCameraZoomEnd = false;
	easingCount_camera = 0;
	cameraDistance = cameraDistance_init;

	isReadyEnd = false;
	easingCount_ready = 0;
	scale_ready = scale_ready_init;

	isGoEnd = false;
	easingCount_go = 0;
	positionX_go = positionX_go_init;

}

void PlayStart::Update()
{
	if (!isActive)
		return;

	//ズームアウト
	if (!isCameraZoomEnd)
		CameraZoomOut();

	//レディーゴー表示
	if (!isReadyEnd)
		ScalingReadySprite();
	else if (!isGoEnd)
		TranslationGoSprite();


	isActive = !isCameraZoomEnd || !isReadyEnd || !isGoEnd;
}

void PlayStart::Draw()
{
	if (!isReadyEnd)
		sp_ready->DrawSprite("GamePlay_Score_Rank", { 1920 / 2, 1080 / 2 }, 0.0f, { scale_ready,scale_ready });
	else if (!isGoEnd)
		sp_go->DrawSprite("GamePlay_Score_Rank", { positionX_go, 1080 / 2 });
}

void PlayStart::CameraZoomOut()
{
	ActorManager* actorManager = ActorManager::GetInstance();
	auto fields = actorManager->GetFields();

	const float cameraDistance_end = 100.0f;
	const XMFLOAT3 cameraTarget_end = fields[0]->GetPosition();
	const float easingCount_end = 120.0f;

	if (easingCount_camera < easingCount_end)
	{
		easingCount_camera++;
		//距離と注視点の移動
		cameraDistance = Easing::EaseInOutQuint(cameraDistance_init, cameraDistance_end, easingCount_end, easingCount_camera);
		camera->SetDistance(cameraDistance);
		XMFLOAT3 target = {
			Easing::EaseInOutQuint(actorManager->GetPlayer()->GetPosition().x, cameraTarget_end.x, easingCount_end, easingCount_camera),
			Easing::EaseInOutQuint(actorManager->GetPlayer()->GetPosition().y, cameraTarget_end.y, easingCount_end, easingCount_camera),
			Easing::EaseInOutQuint(actorManager->GetPlayer()->GetPosition().z, cameraTarget_end.z, easingCount_end, easingCount_camera)
		};
		camera->SetTarget(target);
		camera->Update();
	}
	else
	{
		easingCount_camera = 0;
		isCameraZoomEnd = true;
	}
}

void PlayStart::ScalingReadySprite()
{
	const float scale_end = 1.0f;
	const float easingCount_end = 40.0f;
	const float easingCount_endAdd = 20.0f;//余韻

	easingCount_ready++;
	if (easingCount_ready < easingCount_end)
	{
		scale_ready = Easing::EaseOutExpo(
			scale_ready_init, scale_end,
			easingCount_end, easingCount_ready);
	}
	else if (easingCount_ready > easingCount_end + easingCount_endAdd)
	{
		easingCount_ready = 0;
		isReadyEnd = true;
	}
}

void PlayStart::TranslationGoSprite()
{
	const float position_end = 960.0f;
	const float easingCount_end = 40.0f;
	const float easingCount_endAdd = 20.0f;//余韻

	easingCount_go++;
	if (easingCount_go < easingCount_end)
	{
		positionX_go = Easing::EaseOutExpo(
			positionX_go_init, position_end,
			easingCount_end, easingCount_go);
	}
	else if (easingCount_go > easingCount_end + easingCount_endAdd)
	{
		easingCount_go = 0;
		isGoEnd = true;
	}
}
