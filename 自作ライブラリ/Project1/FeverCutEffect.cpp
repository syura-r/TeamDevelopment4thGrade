#include "FeverCutEffect.h"
#include "ActorManager.h"
#include "Field.h"

FeverCutEffect::FeverCutEffect(Line* arg_line, const DirectX::XMFLOAT4& arg_color)
{
    effectSprite = new Sprite3D();

    line = arg_line;

    scale = Vector2(0.15f, 0.1f);
    color = arg_color;
}

FeverCutEffect::~FeverCutEffect()
{
    delete effectSprite;
}

void FeverCutEffect::Initialize()
{
}

void FeverCutEffect::Upate()
{
    position = line->GetCenter();
    angle = line->GetAngle();
    fieldRotation = ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt();

    const float SCALE_Y_MAX = 0.3f;
    if (scale.y <= SCALE_Y_MAX)
    {
        scale.y += 0.025f;
    }
}

void FeverCutEffect::Draw()
{
    effectSprite->DrawSpriteCutEffect("FeverCuttingEffect", position, angle, fieldRotation, scale, Vector2(0.5f, 0.0f), false, color);
}
