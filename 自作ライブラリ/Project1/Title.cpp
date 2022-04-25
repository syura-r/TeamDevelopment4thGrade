#include "Title.h"

#include "DebugCamera.h"
#include "Sprite.h"
#include "TextureResource.h"

Title::Title()
{
	camera = std::make_unique<DebugCamera>();
	next = SCENE::Play;
	lightGroup.reset(LightGroup::Create());
	lights.clear();
	backTex = std::make_unique<Sprite>();
	resource = std::make_unique<TextureResource>("titleBackTex");
	titleText = std::make_unique<TitleText>();
}


Title::~Title()
{
}

void Title::Initialize()
{
	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());
	camera->SetDistance(0.1f);
	camera->SetTarget({ 0,0,0 });
	isEnd = false;
	titleText->Initialize();
}

void Title::Update()
{
	if(titleText->Update())
	{
		if (titleText->GetStart())
		{
			ShutDown();
		}
		if (titleText->GetEnd())
		{
			
		}
	}
	lightGroup->SetAmbientColor(XMFLOAT3(1,1,1));
	lightGroup->Update();
	camera->AddPhi(0.0002f);
	camera->Update();
	for (auto& it : lights)
	{
		it->Update();
	}

}

void Title::PreDraw()
{
	for (auto& it : lights)
	{
		it->DrawReady();
	}
	if (!Object3D::GetDrawShadow())
	{
		resource->PreDraw();
	}

	for (auto& it : lights)
	{
		it->Draw();
	}

	if (!Object3D::GetDrawShadow())
	{
		resource->PostDraw();
		//DirectXLib::GetInstance()->ReRenderTarget();
		backTex->DrawSprite("titleBackTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex()), { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 },"FisheyeLens");
	}
}

void Title::PostDraw()
{
	if(!Object3D::GetDrawShadow())
	{
		titleText->Draw();
	}
}
