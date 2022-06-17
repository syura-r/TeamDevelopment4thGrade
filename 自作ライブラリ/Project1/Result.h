//#pragma once
//#include "NumberSprite.h"
//#include "Sprite.h"
//#include "TextureResource.h"
//
//class Result
//{
//public:
//	Result();
//	void Initialize(const float SecondTime, const float MinuteTime,const int GetAchieve);
//	bool Update();
//	void Draw();
//	bool GetRetry() { return retryFlag; }
//	bool GetReturnTitle() { return returnTitle; }
//
//private:
//	struct SpriteStruct
//	{
//		SpriteStruct()
//		{
//			spritePtr = std::make_unique<Sprite>();
//			pos = { 0,0 };
//			color = { 1, 1, 1, 1 };
//		}
//		std::unique_ptr<Sprite> spritePtr;
//		Vector2 pos;
//		XMFLOAT4 color;
//	};
//
//	void AppearProc(SpriteStruct& sprite ,const float lastPosY,const int counter,const int MaxTime);
//	int openCounter = 0;
//	int viewCounter = 0;
//	int closeCounter = 0;
//	bool startClose = false;
//
//	
//	SpriteStruct star;
//	SpriteStruct achieve;
//	SpriteStruct getAchieveTex;
//	SpriteStruct result;
//	SpriteStruct half;
//	SpriteStruct retry;
//	SpriteStruct returnTitleTex;
//	SpriteStruct oneMinuteClear;
//	SpriteStruct clearTime;
//	SpriteStruct resultTex;
//	SpriteStruct denialTex;
//
//	std::unique_ptr<NumberSprite> secondTimeSprite;
//	std::unique_ptr<NumberSprite> minuteTimeSprite;
//	std::unique_ptr<Sprite> resultColon;
//	
//	Vector2 timeSpritePos;
//	XMFLOAT4 timeSpriteColor;
//
//	Vector2 denialTexScale;
//	
//	//描画用
//	float drawSecondTime;
//	float drawMinuteTime;
//
//	//実際のクリアタイム保管用
//	float SecondTime;
//	float MinuteTime;
//
//	float timeAddVal;
//
//	bool clearOneMinute = false;
//	bool close = false;
//	int select = 0;
//	int starNum;
//	int getAchieve;
//	int texAlphaCounter;
//
//	const int TexAlphaEasingTime = 45;
//	
//	//タイトルに戻るフラグ
//	bool returnTitle = false;
//	bool retryFlag = false;
//	std::unique_ptr<TextureResource> resource;
//
//	float texAlpha = 0;
//};
//
