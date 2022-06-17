#include "Stadium.h"
#include "OBJLoader.h"
#include"TextureResource.h"
Stadium::Stadium()
{
	Create(OBJLoader::GetModel("stadium"));
	
	position = { 0,-80,0 };
	scale = { 15,15,15 };
	rotation = { 0,90,0 };
	Object::Update();
}

void Stadium::Draw()
{
	PipelineState::SetPipeline("BasicObj");
	Object::Draw();
}

