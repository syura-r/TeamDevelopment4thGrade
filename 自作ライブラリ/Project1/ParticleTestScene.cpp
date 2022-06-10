#include "ParticleTestScene.h"
#include "Input.h"
#include "ParticleEmitter.h"

ParticleTestScene::ParticleTestScene()
{
    next = Scene::SCENE::Play;
    camera = std::make_unique<DebugCamera>();
}

ParticleTestScene::~ParticleTestScene()
{
}

void ParticleTestScene::Initialize()
{
    isEnd = false;
    //ParticleEmitter::Initialize(camera.get());
    ParticleManager::GetInstance()->SetCamera(camera.get());

}

void ParticleTestScene::Update()
{
    camera->Update();

    if (Input::TriggerKey(DIK_SPACE))
    {
        ParticleEmitter::CreateExplosion(Vector3(0, 0, 0));
    }
}

void ParticleTestScene::PreDraw()
{
}

void ParticleTestScene::PostDraw()
{
}
