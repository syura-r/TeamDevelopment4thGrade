#include"LightStruct.hlsli"
cbuffer cbuff0 : register(b0)
{
    matrix viewproj; // ビュープロジェクション行列
    matrix world; // ワールド行列
    float3 cameraPos; // カメラ座標（ワールド座標）
    float4 color;
    uint drawShadow;
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); // アンビエント係数
    float3 m_diffuse : packoffset(c1); // ディフューズ係数
    float3 m_specular : packoffset(c2); // スペキュラー係数
    float m_alpha : packoffset(c2.w); // アルファ
}

cbuffer cbuff3 : register(b3)
{
    //float4 Status;
	
    float _Destruction; //分解度合い
    float _ScaleFactor; //スケールの変化量
    float _PositionFactor; //ポジションの変化量
    float _RotationFactor; //回転の変化量
    uint _Tessellation; //ポリゴン分割度
    uint _OnEasing; //イージングするかどうか
    uint _OnTargetBreak; //特定の位置のみ分解するかどうか
    uint _OnCameraBreak;
    float3 BreakPoint; //分解中心点
    float BreakRadius; //分解半径
    float3 TargetPos;//カメラの注視点
    float CapsuleRadius;//カメラから注視点までのカプセル半径
    uint _On4Vertex;

}
cbuffer cbuff4 : register(b4)
{
    matrix lightCameraVP; //ビュープロジェクション行列
    float3 lightCameraPos; //カメラ座標(ワールド座標)
}

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
    CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldpos : POSITION; // ワールド座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv値
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 shadowPos : TEXCOORD1; //影座標
    float4 worldpos : POSITION; //ワールド座標
    float3 normal : NORMAL; //法線
    float2 uv : TEXCOORD; // uv値
    float destruction : TEXCOORD2;
};

struct PSOutPut
{
    float4 color : SV_TARGET0;//通常のレンダリング結果
    float4 normal : SV_TARGET1;//法線
    float4 depth : SV_TARGET2; //深度値	
};

struct HS_CONTROL_POINT_OUTPUT
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldpos : POSITION; // ワールド座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv値
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3


struct DS_OUTPUT
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 worldpos : POSITION; // ワールド座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv値
};

