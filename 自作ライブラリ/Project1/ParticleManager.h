#pragma once
#include"Particle.h"
#include<forward_list>
#include<unordered_map>
#include<string>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Camera.h"
#include "ComputeShade.h"

class FeverCutEffect;
class ParticleManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�萔�o�b�t�@
	struct ConstBufferData
	{
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	static ParticleManager* GetInstance();
	void Add(Particle* newParticle,const std::string& TexName);
	void Initialize();
	void Update();
	void Draw();
	inline void SetCamera(Camera* camera) { this->camera = camera; }
	void CreateConstBuff();
	void CreateModel();
	void End();

	void AddFeverCutEffect(FeverCutEffect* arg_effect);
	void DeleteFeverCutEffect();
	void UpdateFeverCutEffect();
	void DrawFeverCutEffect();

	void EffectSort();

private:
	ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	~ParticleManager() = default;
	ParticleManager& operator=(const ParticleManager&) = delete;

	int activParticleCount = 0;

	//�X�V���ɕύX����Ȃ��f�[�^
	std::unordered_map<std::string,std::vector<ParticleParameter>>paramData;

	//�X�V���ɕύX����AVS�ɑ�����f�[�^
	std::unordered_map<std::string, std::vector<OutputData>>vertData;

	static const int vertexCount = 10000;

	ComPtr<ID3D12Resource> constBuff;
	Camera* camera = nullptr;
	ComPtr<ID3D12Resource> vertBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView;

	std::unique_ptr<ComputeShade> computeShade;
	std::vector<OutputData> verts;
	std::vector<ParticleParameter> params;
	std::vector<FeverCutEffect*> feverCutEffects;
};