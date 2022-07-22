#pragma once
#include "Sprite3D.h"
#include "Line.h"

class FeverCutEffect
{
public:
    FeverCutEffect(Line* arg_line, const DirectX::XMFLOAT4& arg_color);
    ~FeverCutEffect();
    void Initialize();
    void Upate();
    void Draw();
    const Vector3& GetEffect() { return position; }

private:
    Sprite3D* effectSprite;

    Line* line;

    Vector3 position;
    float angle;
    Vector2 scale;
    Vector3 fieldRotation;

    DirectX::XMFLOAT4 color;
};