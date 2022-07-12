#pragma once
#include "Object.h"
#include "Sprite3D.h"
class PieChart :
    public Object
{
public:
	PieChart(const XMFLOAT4& arg_color1 = { 1,0,0,1 }, const XMFLOAT4& arg_color2 = { 0,0,1,1 });
	//比率が変わった時に使う関数
	void SetColorCount(const int arg_color1Count, const int arg_color2Count);
	//ゲージの色を変えたいときに使う関数
	void SetColor(const XMFLOAT4& arg_color1, const XMFLOAT4& arg_color2)
	{
		color1 = arg_color1;
		color2 = arg_color2;
	}
	void DrawReady()override;
    void Draw()override;

private:
	//比率を計算しバッファに送る
	void ChangeColor();

    int total = 0;
    int color1Count = 0;
	int color2Count = 0;

	struct ConstBufferData
	{
		XMFLOAT4 color1;
		XMFLOAT4 color2;
		float angle;
		XMFLOAT3 pad;
	};
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	//ゲージの色
	XMFLOAT4 color1;
	XMFLOAT4 color2;

	Sprite3D* cover;
};

