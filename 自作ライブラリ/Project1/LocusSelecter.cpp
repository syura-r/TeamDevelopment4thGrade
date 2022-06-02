#include "LocusSelecter.h"
#include "ActorManager.h"
#include "Player.h"

#include <random>
#include <algorithm>

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
	//int a = ActorManager::GetInstance()->GetPlayer()->GetFeverQuota();
}

void LocusSelecter::Draw()
{
	Vector2 scale = { 0.25f,0.25f };
	//B
	if (locusTypes[BBUTTON] != LocusType::UNDIFINED)
	{
		bFrameLocusTex->DrawSprite(GetFileName(locusTypes[BBUTTON]), bTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	}
	frameBtex->DrawSprite("frame_B", bTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });

	//Y
	if (locusTypes[YBUTTON] != LocusType::UNDIFINED)
	{
		yFrameLocusTex->DrawSprite(GetFileName(locusTypes[YBUTTON]), yTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	}
	frameYtex->DrawSprite("frame_Y", yTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	
	//X
	if (locusTypes[XBUTTON] != LocusType::UNDIFINED)
	{
		xFrameLocusTex->DrawSprite(GetFileName(locusTypes[XBUTTON]), xTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	}
	frameXtex->DrawSprite("frame_X", xTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	
	//Next
	if (nextLocusType != LocusType::UNDIFINED)
	{
		nextFrameLocusTex->DrawSprite(GetFileName(nextLocusType), nextTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
	}
	frameNextTex->DrawSprite("frame_next", nextTexPos, 0, scale, { 1,1,1,1 }, { 0,0 });
}

void LocusSelecter::Setting()
{
	vecWaveLocuses.clear();
	nextLocusType = LocusType::UNDIFINED;
	//int max = ActorManager::GetInstance()->GetPlayer()->GetFeverQuota();
	randMax = locusMax - 1;

	for (int i = 0; i < locusMax; i++)
	{
		vecWaveLocuses.push_back((LocusType)i);
	}

	//random‚Å‚»‚ê‚¼‚ê“ü‚ê‚é

	int rand = 0;
	auto itr = vecWaveLocuses.begin();

	rand = GetIntRand(0, randMax);
	locusTypes[XBUTTON] = vecWaveLocuses.at(rand);
	vecWaveLocuses.erase(itr + rand);
	randMax--;

	itr = vecWaveLocuses.begin();
	rand = GetIntRand(0, randMax);
	locusTypes[YBUTTON] = vecWaveLocuses.at(rand);
	vecWaveLocuses.erase(itr + rand);
	randMax--;

	itr = vecWaveLocuses.begin();
	rand = GetIntRand(0, randMax);
	locusTypes[BBUTTON] = vecWaveLocuses.at(rand);
	vecWaveLocuses.erase(itr + rand);
	randMax--;

	if (vecWaveLocuses.size() > 0)
	{
		itr = vecWaveLocuses.begin();
		rand = GetIntRand(0, randMax);
		nextLocusType = vecWaveLocuses.at(rand);
		vecWaveLocuses.erase(itr + rand);
		randMax--;
	}
	else
	{
		nextLocusType = LocusType::UNDIFINED;
	}
	//Œã‚ÅŽ¡‚·
	vecWaveLocuses.clear();
	for (int i = 0; i < locusMax; i++)
	{
		vecWaveLocuses.push_back((LocusType)i);
	}
	randMax = locusMax - 1;
}

void LocusSelecter::SetNextLocus(Button arg_pressedButton)
{
	locusTypes[arg_pressedButton] = nextLocusType;

	int rand = 0;
	while (true)
	{
		rand = GetIntRand(0, randMax);
		nextLocusType = vecWaveLocuses.at(rand);
		if (locusTypes[XBUTTON] != nextLocusType &&
			locusTypes[YBUTTON] != nextLocusType &&
			locusTypes[BBUTTON] != nextLocusType)
		{
			break;
		}
	}

	/*if (vecWaveLocuses.size() > 0)
	{
		auto itr = vecWaveLocuses.begin();
		int rand = GetIntRand(0, randMax);
		nextLocusType = vecWaveLocuses.at(rand);
		vecWaveLocuses.erase(itr + rand);
		randMax--;
	}
	if (vecWaveLocuses.size() <= 0)
	{
		for (int i = 0; i < locusMax; i++)
		{
			vecWaveLocuses.push_back((LocusType)i);
			
		}
		randMax = locusMax - 1;
	}*/
}

LocusType LocusSelecter::XbuttonLocusType()
{
	return locusTypes[XBUTTON];
}

LocusType LocusSelecter::YbuttonLocusType()
{
	return locusTypes[YBUTTON];
}

LocusType LocusSelecter::BbuttonLocusType()
{
	return locusTypes[BBUTTON];
}

std::string LocusSelecter::GetFileName(LocusType arg_locusType)
{
	std::string fileName;
	switch (arg_locusType)
	{
	case LocusType::UNDIFINED:
		fileName = "noLocusTypeError";
		break;
	case LocusType::TRIANGLE:
		fileName = "shape_triangle";
		break;
	case LocusType::RIBBON:
		fileName = "shape_ribbon";
		break;
	case LocusType::PENTAGON:
		fileName = "shape_pentagon";
		break;
	case LocusType::STAR:
		fileName = "shape_star";
		break;
	case LocusType::HEXAGRAM:
		fileName = "shape_hexagram";
		break;
	case LocusType::TRIFORCE:
		fileName = "shape_triforce";
		break;
	default:
		break;
	}
	
    return fileName;
}

int LocusSelecter::GetIntRand(int minValue, int maxValue)
{
	std::random_device rnd;
	std::mt19937_64 mt64(rnd());
	std::uniform_int_distribution<int> genRandFloat(minValue, maxValue);
	return genRandFloat(mt64);
}
