
#include "Sprite3D.h"
#include<math.h>
#include<d3dx12.h>
#include"Texture.h"
#include"Camera.h"
#include "Object3D.h"
Camera* Sprite3D::camera = nullptr;
Sprite3D::Sprite3D()
{
	CreateSprite3D();
}


void Sprite3D::CreateSprite3D()
{
	HRESULT result;
	vertices[0].normal = { 0.0f,0.0f,1.0f };
	vertices[1].normal = { 0.0f,0.0f,1.0f };
	vertices[2].normal = { 0.0f,0.0f,1.0f };
	vertices[3].normal = { 0.0f,0.0f,1.0f };

	vertices[0].uv = { 0.0f,0.0f };
	vertices[1].uv = { 0.0f,1.0f };
	vertices[2].uv = { 1.0f,0.0f };
	vertices[3].uv = { 1.0f,1.0f };

	auto dev = DirectXLib::GetInstance()->GetDevice();

	//���_�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
	//�萔�o�b�t�@�̍쐬
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff2));
	if (FAILED(result)) {
		assert(0);
	}

	D3D12_HEAP_PROPERTIES heapprop{};//���_�q�[�v�ݒ�
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	D3D12_RESOURCE_DESC resdesc{};  //���\�[�X�ݒ�
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeof(indices); //���_��񂪓��镪�̃T�C�Y
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < 6; i++)
	{
		indexMap[i] = indices[i];  //�C���f�b�N�X���R�s�[
	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);


}
void Sprite3D::DrawSprite(const std::string& name, const Vector3& position, const float& rotation, const XMFLOAT2& scale, const XMFLOAT2& anchorPoint, bool billboard)
{
	DrawSprite(name, position, rotation, scale, { 1,1,1,1 }, anchorPoint, "NoShade", ALPHA, billboard);
}

void Sprite3D::DrawSprite(const std::string& name, const Vector3& position, const float& rotation, const XMFLOAT2& scale, const XMFLOAT4& color, const XMFLOAT2& anchorPoint, bool billboard)
{
	DrawSprite(name, position, rotation, scale, color, anchorPoint, "NoShade", ALPHA, billboard);
}

void Sprite3D::DrawSprite(const std::string& name, const Vector3& position, const float& rotation, const XMFLOAT2& scale, const XMFLOAT4& color, const XMFLOAT2& anchorPoint, const std::string& pipelineName, BLENDTYPE type,bool billboard)
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();

	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();
	float width = (float)resDesc.Width; //�摜�̉���
	float height = (float)resDesc.Height; //�摜�̏c��
	HRESULT result;
	vertices[0].pos = { (0.0f - anchorPoint.x) * 0.1f,(1.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[1].pos = { (0.0f - anchorPoint.x) * 0.1f,(0.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[2].pos = { (1.0f - anchorPoint.x) * 0.1f,(1.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[3].pos = { (1.0f - anchorPoint.x) * 0.1f,(0.0f - anchorPoint.y) * 0.1f,0.0f };
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	memcpy(vertMap, vertices.data(), sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���[���h�s��̍X�V
	XMMATRIX matScale, matRot, matTrans;
	if (rect)
	{
		matScale = XMMatrixScaling(scale.x * rectTexSize.x, scale.y * rectTexSize.y, 1);
	}
	else
	{
		matScale = XMMatrixScaling(scale.x * width, scale.y * height, 1);
	}
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= matScale;
	spriteMatWorld *= matRot;
	if (billboard)
	{
		const XMMATRIX& matBillboard = camera->GetMatBillboard();
		if (parent == nullptr) {
			spriteMatWorld *= matBillboard;
		}
	}
	spriteMatWorld *= matTrans;

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		spriteMatWorld *= parent->spriteMatWorld;
	}

	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	const XMFLOAT3& cameraPos = camera->GetEye();
	if (!Object3D::GetScreenDraw())
	{
		// �萔�o�b�t�@�փf�[�^�]��
		ConstBufferData* constMap = nullptr;
		result = constBuff->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		constMap->viewprojection = matViewProjection;
		constMap->cameraPos = cameraPos;
		constMap->world = spriteMatWorld;
		constMap->color = color;
		constBuff->Unmap(0, nullptr);
	}
	else
	{
		ConstBufferData* constMap2 = nullptr;
		result = constBuff2->Map(0, nullptr, (void**)&constMap2);
		assert(SUCCEEDED(result));
		constMap2->viewprojection = matViewProjection;
		constMap2->cameraPos = cameraPos;
		constMap2->world = spriteMatWorld;
		constMap2->color = color;
		constBuff2->Unmap(0, nullptr);
	}

	PipelineState::SetPipeline(pipelineName, type);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
	if (!Object3D::GetScreenDraw())
	{
		cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	}
	else
	{
		cmdList->SetGraphicsRootConstantBufferView(0, constBuff2->GetGPUVirtualAddress());
	}
	//�C���f�b�N�X�o�b�t�@�̃Z�b�g�R�}���h
	cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV(name));

	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);


}

void Sprite3D::DrawSpriteCutEffect(const std::string& name, const Vector3& position, const float& rotation, const Vector3& fieldRotation, const XMFLOAT2& scale, const XMFLOAT2& anchorPoint, bool billboard, const DirectX::XMFLOAT4& arg_color)
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();

	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();
	float width = (float)resDesc.Width; //�摜�̉���
	float height = (float)resDesc.Height; //�摜�̏c��
	HRESULT result;
	vertices[0].pos = { (0.0f - anchorPoint.x) * 0.1f,(1.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[1].pos = { (0.0f - anchorPoint.x) * 0.1f,(0.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[2].pos = { (1.0f - anchorPoint.x) * 0.1f,(1.0f - anchorPoint.y) * 0.1f,0.0f };
	vertices[3].pos = { (1.0f - anchorPoint.x) * 0.1f,(0.0f - anchorPoint.y) * 0.1f,0.0f };
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	memcpy(vertMap, vertices.data(), sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���[���h�s��̍X�V
	XMMATRIX matScale, matRot, matTrans;
	if (rect)
	{
		matScale = XMMatrixScaling(scale.x * rectTexSize.x, scale.y * rectTexSize.y, 1);
	}
	else
	{
		matScale = XMMatrixScaling(scale.x * width, scale.y * height, 1);
	}
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation));
	matRot *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(fieldRotation.x), XMConvertToRadians(fieldRotation.y), XMConvertToRadians(fieldRotation.z));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= matScale;
	spriteMatWorld *= matRot;
	if (billboard)
	{
		const XMMATRIX& matBillboard = camera->GetMatBillboard();
		if (parent == nullptr) {
			spriteMatWorld *= matBillboard;
		}
	}
	spriteMatWorld *= matTrans;

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		spriteMatWorld *= parent->spriteMatWorld;
	}

	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	const XMFLOAT3& cameraPos = camera->GetEye();
	if (!Object3D::GetScreenDraw())
	{
		// �萔�o�b�t�@�փf�[�^�]��
		ConstBufferData* constMap = nullptr;
		result = constBuff->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		constMap->viewprojection = matViewProjection;
		constMap->cameraPos = cameraPos;
		constMap->world = spriteMatWorld;
		constMap->color = arg_color;
		constBuff->Unmap(0, nullptr);
	}
	else
	{
		ConstBufferData* constMap2 = nullptr;
		result = constBuff2->Map(0, nullptr, (void**)&constMap2);
		assert(SUCCEEDED(result));
		constMap2->viewprojection = matViewProjection;
		constMap2->cameraPos = cameraPos;
		constMap2->world = spriteMatWorld;
		constMap2->color = arg_color;
		constBuff2->Unmap(0, nullptr);
	}

	PipelineState::SetPipeline("NoShade", ALPHA);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
	if (!Object3D::GetScreenDraw())
	{
		cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	}
	else
	{
		cmdList->SetGraphicsRootConstantBufferView(0, constBuff2->GetGPUVirtualAddress());
	}
	//�C���f�b�N�X�o�b�t�@�̃Z�b�g�R�}���h
	cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV(name));

	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);


}

void Sprite3D::SpriteSetTextureRect(const std::string& name, const float& tex_x, const float& tex_y, const float& tex_width, const float& tex_height)
{

	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(name).Get()->GetDesc();

	float tex_left = tex_x / resDesc.Width;
	float tex_right = (tex_x + tex_width) / resDesc.Width;
	float tex_top = tex_y / resDesc.Height;
	float tex_bottom = (tex_y + tex_height) / resDesc.Height;

	vertices[0].uv = { tex_left,tex_top };
	vertices[1].uv = { tex_left,tex_bottom };
	vertices[2].uv = { tex_right,tex_top };
	vertices[3].uv = { tex_right,tex_bottom };

	rectTexSize.x = tex_width;
	rectTexSize.y = tex_height;


	rect = true;
}
