#pragma once
#include "Object.h"
class Stadium :
    public Object
{
public:
    Stadium();
    ~Stadium();
    void Initialize()override;
    void Update()override;
    void Draw()override;
private:
    Object* lavaObj;
    float uvOffset;
    float offsetSpeed;
    int speedChangeTimer;
	struct ConstBufferData
	{
        float uvOffset;
        XMFLOAT3 pad;

	};
    ComPtr<ID3D12Resource> constBuff; // 定数バッファ

};

