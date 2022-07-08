#pragma once
#include "Sprite3D.h"
#include "Line.h"

class FeverCutEffect
{
public:
    FeverCutEffect(Line* arg_line);
    ~FeverCutEffect();
    void Initialize();
    void Upate();
    void Draw();

private:
    Sprite3D* effectSprite;

    Line* line;

    Vector3 position;
    float angle;
    Vector2 scale;
    Vector3 fieldRotation;
};