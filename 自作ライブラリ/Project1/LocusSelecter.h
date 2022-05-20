#pragma once

#include "Sprite.h"
#include "LocusDef.h"

class LocusSelecter
{
	
public:
	LocusSelecter();
	~LocusSelecter();

	void Initialize();
	void Update();
	void Draw();

	std::string GetFileName(LocusType arg_locusType);

private:
	LocusType locusTypes[3]; //0 B ,1 Y ,2 X

	LocusType nextLocusType;

	Sprite* frameXtex;
	Sprite* frameYtex;
	Sprite* frameBtex;
	Sprite* frameNextTex;

	Sprite* xFrameLocusTex;
	Sprite* yFrameLocusTex;
	Sprite* bFrameLocusTex;
	Sprite* nextFrameLocusTex;

	Vector2 xTexPos;
	Vector2 yTexPos;
	Vector2 bTexPos;
	Vector2 nextTexPos;

};

