#pragma once
#include "Object.h"
#include "Sprite3D.h"
class PieChart :
    public Object
{
public:
	PieChart(const XMFLOAT4& arg_color1 = { 1,0,0,1 }, const XMFLOAT4& arg_color2 = { 0,0,1,1 });
	//�䗦���ς�������Ɏg���֐�
	void SetColorCount(const int arg_color1Count, const int arg_color2Count);
	//�Q�[�W�̐F��ς������Ƃ��Ɏg���֐�
	void SetColor(const XMFLOAT4& arg_color1, const XMFLOAT4& arg_color2)
	{
		color1 = arg_color1;
		color2 = arg_color2;
	}
	void DrawReady()override;
    void Draw()override;

private:
	//�䗦���v�Z���o�b�t�@�ɑ���
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
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	//�Q�[�W�̐F
	XMFLOAT4 color1;
	XMFLOAT4 color2;

	Sprite3D* cover;
};

