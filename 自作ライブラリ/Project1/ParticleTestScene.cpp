#include "ParticleTestScene.h"
#include "Input.h"

ParticleTestScene::ParticleTestScene()
{
    next = Scene::SCENE::Play;
}

ParticleTestScene::~ParticleTestScene()
{
}

void ParticleTestScene::Initialize()
{
    isEnd = false;
}

void ParticleTestScene::Update()
{
}

void ParticleTestScene::PreDraw()
{
}

void ParticleTestScene::PostDraw()
{
}
