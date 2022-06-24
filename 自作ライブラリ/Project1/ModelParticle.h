#pragma once
#include "Vector.h"
#include "Object.h"

//コンピュートシェーダを使用しないFBXなどのモデルを用いたパーティクル用基底クラス
//多用厳禁
class ModelParticle : public Object
{
public:
    ModelParticle();
    ~ModelParticle();
    void Initialize() override;
    void Update() override;
    void Draw() override;

protected:
};