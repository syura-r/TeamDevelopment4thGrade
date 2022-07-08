#include "Stadium.h"
#include "OBJLoader.h"
#include"TextureResource.h"
Stadium::Stadium()
{
	Create(OBJLoader::GetModel("stadium"));
	
	position = { 0,-80,0 };
	scale = { 15,15,15 };
	rotation = { 0,90,0 };
	Object::Update();

	auto result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	lavaObj = new Object();
	lavaObj->Create(OBJLoader::GetModel("lava"));
	lavaObj->SetPosition(position);
	lavaObj->SetScale({65,1,65});
	lavaObj->SetRotation({0,45,0});
	lavaObj->Update();

	Initialize();
}

Stadium::~Stadium()
{
	delete lavaObj;
}

void Stadium::Initialize()
{
	offsetSpeed = 0.0001f * (std::rand() % 10);
	speedChangeTimer = 0;
	uvOffset = 0;
}

void Stadium::Update()
{
	speedChangeTimer++;
	if (speedChangeTimer == 30)
	{
		offsetSpeed = 0.0003f;
	}
	if (speedChangeTimer >= 60)
	{
		offsetSpeed = 0.0006f;
		speedChangeTimer = 0;
	}

	uvOffset += offsetSpeed;

	if (uvOffset >= 1.0f)
		uvOffset = 0;
	ConstBufferData* constMap = nullptr;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->uvOffset = uvOffset;
	constBuff->Unmap(0, nullptr);

	lavaObj->SetPosition(position);
	lavaObj->Update();
	Object::Update();
}

void Stadium::Draw()
{
	PipelineState::SetPipeline("UVScrolling");
	DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
	lavaObj->CustomDraw(false, false);

	PipelineState::SetPipeline("BasicObj");
	Object::Draw();
}

