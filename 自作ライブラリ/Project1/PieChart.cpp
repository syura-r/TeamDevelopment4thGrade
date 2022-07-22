#include "PieChart.h"
#include"OBJModel.h"
#include"OBJLoader.h"
#include "Texture.h"

PieChart::PieChart(const XMFLOAT4& arg_color1, const XMFLOAT4& arg_color2):color1(arg_color1),color2(arg_color2)
{

	Create(OBJLoader::GetModel("plate"));
	billboardType = NORMAL;

	HRESULT result;
	// 定数バッファの生成
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	cover = new Sprite3D();
}

PieChart::~PieChart()
{
	delete 	cover;
}

void PieChart::SetColorCount(const int arg_color1Count, const int arg_color2Count)
{
	color1Count = arg_color1Count;
	color2Count = arg_color2Count;
	ChangeColor();
}

void PieChart::ChangeColor()
{
	total = color1Count + color2Count;
	float angle = 360 * ((float)color2Count/(float)total) * XM_PI / 180;
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->angle = angle;
	constMap->color1 = color1;
	constMap->color2 = color2;

	constBuff->Unmap(0, nullptr);
	
}


void PieChart::DrawReady()
{
	//ImGui::Begin("Chart");
	//ImGui::SliderInt("blue", &color2Count, 0, 256);
	//ImGui::SliderInt("red", &color1Count, 0, 256);
	//ImGui::End();
	//ChangeColor();

	if (total > 0)
	{
		//cover->DrawSprite("GamePlay_CircleCover", position, 0.0f, { scale.x / 16, scale.x / 16 });
	}

	pipelineName = "PieChart";
}

void PieChart::Draw()
{
	if (total <= 0)
		return;
	PipelineState::SetPipeline("PieChart");
	DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
	Object::CustomDraw();
}
