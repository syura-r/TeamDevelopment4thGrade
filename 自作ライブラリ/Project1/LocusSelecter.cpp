#include "LocusSelecter.h"

LocusSelecter::LocusSelecter()
{
	frameBtex = new Sprite();
	frameXtex = new Sprite();
	frameYtex = new Sprite();
	frameNextTex = new Sprite();

	xFrameLocusTex = new Sprite();
	yFrameLocusTex = new Sprite();
	bFrameLocusTex = new Sprite();
	nextFrameLocusTex = new Sprite();
}

LocusSelecter::~LocusSelecter()
{
	delete frameBtex;
	delete frameXtex;
	delete frameYtex;
	delete frameNextTex;
	delete xFrameLocusTex;
	delete yFrameLocusTex;
	delete bFrameLocusTex;
	delete nextFrameLocusTex;
}

void LocusSelecter::Initialize()
{
	bTexPos = { 1772,50 };
	yTexPos = { 1644,50 };
	xTexPos = { 1516,50 }; 
	nextTexPos = { 1644,250 };
}

void LocusSelecter::Update()
{
}

void LocusSelecter::Draw()
{
	Vector2 scale = { 0.25f,0.25f };
	//B
	frameBtex->DrawSprite("frame_B", bTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	//bFrameLocusTex->DrawSprite(GetFileName(locusTypes[0]), bTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });

	//Y
	frameYtex->DrawSprite("frame_Y", yTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	//yFrameLocusTex->DrawSprite(GetFileName(locusTypes[1]), yTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });

	//X
	frameXtex->DrawSprite("frame_X", xTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	//xFrameLocusTex->DrawSprite(GetFileName(locusTypes[2]), xTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });

	frameNextTex->DrawSprite("frame_next", nextTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	//nextFrameLocusTex->DrawSprite(GetFileName(nextLocusType), nextTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
}

std::string LocusSelecter::GetFileName(LocusType arg_locusType)
{
	std::string fileName;
	switch (arg_locusType)
	{
	case LocusType::UNDIFINED:
		fileName = "";
		break;
	case LocusType::STAR:
		fileName = "shape_star";
		break;
	case LocusType::RIBBON:
		fileName = "shape_ribbon";
		break;
	case LocusType::TRIFORCE:
		fileName = "shape_triforce";
		break;
	default:
		break;
	}
    return fileName;
}
