#include "SceneManager.h"

#include "Easing.h"
#include "Object3D.h"
#include "Sprite.h"
#include "TextureResource.h"
#include "TransitionTriangle.h"


SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	//instance.Initialize();
	return &instance;
}

void SceneManager::Add(Scene::SCENE name, Scene * scene)
{
	if (scenes[name] != nullptr)
	{
		return;
	}
	scenes[name].reset(scene);
}

void SceneManager::Initialize()
{
	resource.reset(new TextureResource("migrateTex", { 1920,1080 }, DXGI_FORMAT_R8G8B8A8_UNORM, { 0,0,0,0 }, false));
	migrateTex = std::make_unique<Sprite>();
	migrateStart = false;
	//定数バッファの作成
	auto result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDate) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	migrateTime = 1.0f;
	migrateCounter = 0;

	transitionTriangle = std::make_unique<TransitionTriangle>();
	transitionTriangle.get()->Initialize();
}

void SceneManager::Update()
{
	if (currentScene == nullptr)
	{
		return;
	}
	//if(migrateStart)
	//{
	//	if (migrateCounter < 60)
	//		migrateTime = Easing::EaseInOutQuad(1, 0, 60, migrateCounter);
	//	else
	//		migrateTime = Easing::EaseInOutQuad(0, 1, 60, migrateCounter-60);
	//	migrateCounter++;
	//	if (migrateCounter == 60)
	//	{
	//		Change(currentScene->NextScene());
	//		currentScene->Update();
	//	}
	//	if(migrateCounter == 120)
	//	{
	//		migrateStart = false;
	//		migrateCounter = 0;
	//	}
	//	return;
	//}
	//if (currentScene->GetIsEnd())
	//{
	//	migrateStart = true;
	//}

	const bool prevIsClose = transitionTriangle.get()->GetIsClose();
	const bool prevIsOpen = transitionTriangle.get()->GetIsOpen();

	if (prevIsClose || prevIsOpen)
	{
		transitionTriangle.get()->Update();

		if (prevIsClose &&
			!transitionTriangle.get()->GetIsClose())
		{
			Change(currentScene->NextScene());
			currentScene->Update();
			transitionTriangle.get()->IsOpen();
		}
		return;
	}

	currentScene->Update();
	if (currentScene->GetIsEnd())
	{
		transitionTriangle.get()->IsClose();
	}
}

void SceneManager::Change(Scene::SCENE name)
{
	currentScene = scenes[name].get();
	currentScene->Initialize();
}

void SceneManager::PreDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}


	//if (((!migrateStart && currentScene->GetIsEnd()) || migrateCounter == 60) && !Object3D::GetDrawShadow())
	//	resource->PreDraw();
	if (!migrateStart || migrateCounter == 60)
		currentScene->PreDraw();
}

void SceneManager::PostDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if (!migrateStart || migrateCounter == 60)
		currentScene->PostDraw();
	PipelineState::SetPipeline("Sprite");
	transitionTriangle.get()->Draw();
	//if (((!migrateStart && currentScene->GetIsEnd()) || migrateCounter == 60) && !Object3D::GetDrawShadow())
	//{
	//	resource->PostDraw();
	//}
	//if(migrateStart|| currentScene->GetIsEnd() && !Object3D::GetDrawShadow())
	//{
	//	auto libPtr = DirectXLib::GetInstance();
	//	ConstBufferDate* constMap = nullptr;
	//	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	//	constMap->migrateTime = migrateTime;
	//	constBuff->Unmap(0, nullptr);

	//	PipelineState::SetPipeline("Migrate");
	//	libPtr->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	//	migrateTex->NoPipelineDraw("migrateTex", { 960,540 }, 0, { 1,1 }, { 1,1,1,1 }, { 0.5f, 0.5f });
	//}
}
