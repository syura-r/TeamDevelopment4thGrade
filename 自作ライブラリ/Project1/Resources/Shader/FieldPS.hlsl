#include"BasicShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float> tex1 : register(t1); // 1番スロットに設定されたテクスチャ

SamplerState smp0 : register(s0); // 0番スロットに設定されたサンプラー
SamplerState smp1 : register(s1); // 1番スロットに設定されたサンプラー

float4 PSmain(VSOutput input) : SV_TARGET
{


    //const float zlnLVP = input.svpos.z / input.svpos.w;
	
    float2 locusMapUV /*= input.shadowPos.xy / input.shadowPos.w*/;
    locusMapUV.x = input.svpos.x /1920.0f;
    locusMapUV.y = input.svpos.y / 1080.0f;
	
    //float locus = tex1.SampleCmpLevelZero(smp1, locusMapUV, zlnLVP);
    float locus = tex1.Sample(smp1, locusMapUV);
    if(locus > 0)
    {
        discard;
    }
        	//テクスチャマッピング
    float4 colorSRGB = tex0.Sample(smp0, input.uv);
    float4 texcolor = pow(colorSRGB, 2.2);

	//シェーディングによる色で描画
    float4 returnColor = pow(( texcolor * color), 1.0 / 2.2);
    return returnColor;

}