#pragma once
#include <memory>

#include "Scene.h"
#include "DebugCamera.h"

class ParticleTestScene : public Scene
{
public:
    ParticleTestScene();
    ~ParticleTestScene();
    void Initialize() override;
    void Update() override;
    void PreDraw() override;
    void PostDraw() override;

private:
    std::unique_ptr<DebugCamera> camera;
};