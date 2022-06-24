#include "ModelParticle.h"
#include "OBJLoader.h"

ModelParticle::ModelParticle()
{
    position = Vector3(0, 0, 0);
    Create(OBJLoader::GetModel("fieldPiece"));

    Initialize();
}

ModelParticle::~ModelParticle()
{
}

void ModelParticle::Initialize()
{
    rotation = Vector3(0, 0, 0);
    scale = Vector3(1, 1, 1);
    Object::Update();
}

void ModelParticle::Update()
{
    position.y += 1;

    Object::Update();
}

void ModelParticle::Draw()
{
    Object::Draw();
}