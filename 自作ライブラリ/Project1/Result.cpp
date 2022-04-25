#include "Result.h"

#include "Easing.h"
#include "Input.h"

Result::Result()
{
	resource.reset(new TextureResource("ResultTex",{700,960}, DXGI_FORMAT_R8G8B8A8_UNORM,{0,0,0,1}));
	resultTex.pos = { 960,540 };
	result.pos = { 350,100 };
	clearTime.pos = { 220,280 };
	oneMinuteClear.pos = { 350,420 };
	achieve.pos = { 260,550 };
	star.pos = { 350,690 };
	star.color = { 1, 1, 1, 0 };
	retry.pos = { 175,870 };
	returnTitleTex.pos = { 525,870 };
	half.pos = { 500,550 };
	getAchieveTex.pos = { 450,550 };
	getAchieveTex.color = { 1,1,1,0 };

	secondTimeSprite  = std::make_unique<NumberSprite>(drawSecondTime);
	minuteTimeSprite  = std::make_unique<NumberSprite>(drawMinuteTime);
	resultColon = std::make_unique<Sprite>();

	timeSpritePos = {500,280};
	timeSpriteColor = {1,1,1,0};

	denialTexScale = { 0,3 };
	denialTex.pos = { 100,420 };
	denialTex.color = { 0.6f,0.6f,0.6f,1 };

}

void Result::Initialize(const float SecondTime, const float MinuteTime, const int GetAchieve)
{
	close = false;
	starNum = 1;
	oneMinuteClear.color = { 1,1,1,1 };
	retry.color = { 1,1,1,0 };
	returnTitleTex.color = { 1,1,1,0 };
	getAchieveTex.color = { 1,1,1,0 };
	denialTexScale = { 0,3 };
	star.color = { 1, 1, 1, 0 };
	denialTex.color = { 0.6f,0.6f,0.6f,1 };
	timeSpriteColor = { 1,1,1,0 };
	openCounter = 0;
	viewCounter = 0;
	closeCounter = 0;
	startClose = false;
	drawMinuteTime = 0;
	drawSecondTime = 0;
	this->SecondTime = SecondTime;
	this->MinuteTime = MinuteTime;
	timeAddVal = (MinuteTime * 6 + SecondTime) / 60.0f;
	if(MinuteTime <= 0)
	{
		clearOneMinute = true;
		starNum++;
	}
	else
	{
		clearOneMinute = false;
	}
	if (MinuteTime <= 2) 
		starNum++;
	getAchieve = GetAchieve;
	starNum += GetAchieve;
}

bool Result::Update()
{

	//発生時のフェード処理
	if (openCounter <= 90)
	{
		resultTex.color.w = Easing::EaseInOutCubic(0.0f, 0.8f, 90, openCounter);
		resultTex.pos.y = Easing::EaseInOutQuart(540 + 100, 540, 90, openCounter);
		openCounter++;
		return false;
	}

	//表示状態
		//クローズ時の縮小処理
	if (startClose)
	{
		resultTex.color.w = Easing::EaseInOutCubic(0.8f, 0.0f, 90, closeCounter);
		resultTex.pos.y = Easing::EaseInOutQuart(540, 540 + 100, 90, closeCounter);
		closeCounter++;
		if (closeCounter > 90)
			close = true;
		return close;
	}

	if (viewCounter == 0)
	{
		timeSpriteColor = { 1,1,1,1 };
	}
	if (viewCounter < 60)
	{
		drawSecondTime += timeAddVal;
		if (drawSecondTime >= 6)
		{
			drawSecondTime -= 6.0f;
			drawMinuteTime += 1.0f;
		}
		if (viewCounter == 59)
		{
			drawSecondTime = SecondTime;
			drawMinuteTime = MinuteTime;
		}
	}
	else if (viewCounter < 90)
	{
		//1分以内にクリアできてるか
		if (clearOneMinute)
		{

		}
		else
		{
			denialTexScale.x = Easing::EaseOutQuart(0, 500, 30, viewCounter - 60);
			auto color = Easing::EaseInQuart(1, 0.4f, 30, viewCounter - 60);
			oneMinuteClear.color = { color,color,color,1 };
		}
	}
	else if (viewCounter == 120)
	{
		getAchieveTex.spritePtr->SpriteSetTextureRect("resultNumber", 31 * getAchieve, 0, 31, 63);
		getAchieveTex.color = { 1,1,1,1 };
	}
	else if (viewCounter == 150)
	{
		star.color = {1, 1, 1, 1};
		star.spritePtr->SpriteSetTextureRect("star", 0, 0, 93, 77);
		if(starNum==1)
		{
			viewCounter += 75;
		}
	}
	else if (viewCounter == 165)
	{
		star.spritePtr->SpriteSetTextureRect("star", 0, 0, 93 * 2, 77);
		if (starNum == 2)
		{
			viewCounter += 60;
		}

	}
	else if (viewCounter == 180)
	{
		star.spritePtr->SpriteSetTextureRect("star", 0, 0, 93 * 3, 77);
		if (starNum == 3)
		{
			viewCounter += 45;
		}
	}
	else if (viewCounter == 195)
	{
		star.spritePtr->SpriteSetTextureRect("star", 0, 0, 93 * 4, 77);
		if (starNum == 4)
		{
			viewCounter += 30;
		}
	}
	else if (viewCounter == 210)
	{
		star.spritePtr->SpriteSetTextureRect("star", 0, 0, 93 * 5, 77);
	}
	else if(viewCounter == 225)
	{
		star.color = { 1,1,0,1 };
	}
	else if (viewCounter == 255)
	{
		retry.color = { 1,1,1,1 };
		returnTitleTex.color = { 1,1,1,1 };
		texAlphaCounter = 0;
	}
	if (viewCounter > 255)
	{
		if (select == 0 && (Input::TriggerKey(DIK_D) || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerPadLStickRight()))
		{
			select = 1;
			texAlphaCounter = 0;
			retry.color = { 1,1,1,1 };
		}
		else if (select == 1 && (Input::TriggerKey(DIK_A) || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerPadLStickLeft()))
		{
			select = 0;
			texAlphaCounter = 0;
			returnTitleTex.color = { 1,1,1,1 };
		}
		//リトライ選択中
		if (select == 0)
		{
			if (texAlphaCounter <= TexAlphaEasingTime)
				retry.color.w = Easing::EaseInQuart(1, 0, TexAlphaEasingTime, texAlphaCounter);
			else
				retry.color.w = Easing::EaseOutQuart(0, 1, TexAlphaEasingTime, texAlphaCounter - TexAlphaEasingTime);
			if(Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_B))
			{
				startClose = true;
				retryFlag = true;
			}
		}
		//タイトルに戻る選択中
		else
		{
			if (texAlphaCounter <= TexAlphaEasingTime)
				returnTitleTex.color.w = Easing::EaseInQuart(1, 0, TexAlphaEasingTime, texAlphaCounter);
			else
				returnTitleTex.color.w = Easing::EaseOutQuart(0, 1, TexAlphaEasingTime, texAlphaCounter - TexAlphaEasingTime);
			if (Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_B))
			{
				startClose = true;
				returnTitle = true;
			}

		}
		texAlphaCounter++;
		if (texAlphaCounter > TexAlphaEasingTime * 2)
			texAlphaCounter = 0;
	}
	else
		viewCounter++;

	return false;
}

void Result::Draw()
{
	resource->PreDraw();
	result.spritePtr->DrawSprite("result", result.pos, 0, { 1,1 }, result.color);
	clearTime.spritePtr->DrawSprite("clearTime", clearTime.pos, 0, { 1,1 }, clearTime.color);

	secondTimeSprite->Draw(2, "resultNumber", timeSpritePos + Vector2{ 30,0 }, { 1.2f,1.2f }, timeSpriteColor, { 0,0.5f });
	int digits = 1;
	if (drawMinuteTime >= 10) digits = 2;
	minuteTimeSprite->Draw(digits, "resultNumber", timeSpritePos - Vector2{ 30,0 }, { 1.2f,1.2f }, timeSpriteColor, { 1,0.5f });
	resultColon->DrawSprite("resultColon", timeSpritePos, 0, { 1.2f,1.2f }, timeSpriteColor);
	oneMinuteClear.spritePtr->DrawSprite("1MinuteClear", oneMinuteClear.pos, 0, { 1,1 }, oneMinuteClear.color);
	denialTex.spritePtr->DrawSprite("white1x1", denialTex.pos, 0, denialTexScale, denialTex.color, { 0,0.5f });

	achieve.spritePtr->DrawSprite("achieve", achieve.pos, 0, { 1,1 }, achieve.color);
	half.spritePtr->DrawSprite("half", half.pos, 0, { 1,1 }, half.color);
	getAchieveTex.spritePtr->DrawSprite("resultNumber", getAchieveTex.pos, 0, { 1,1 }, getAchieveTex.color);

	star.spritePtr->DrawSprite("star", star.pos, 0, { 1,1 }, star.color);
	retry.spritePtr->DrawSprite("retry", retry.pos, 0, { 1,1 }, retry.color);
	returnTitleTex.spritePtr->DrawSprite("returnTitle", returnTitleTex.pos, 0, { 1,1 }, returnTitleTex.color);
	//リソースバリアを戻す
	resource->PostDraw();
	//レンダリングされたメニュー画像を描画
	resultTex.spritePtr->DrawSprite("ResultTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex()), resultTex.pos, 0, { 1,1 }, resultTex.color, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Result::AppearProc(SpriteStruct& sprite, const float lastPosY, const int counter, const int MaxTime)
{
	sprite.color.w = Easing::EaseInOutQuart(0, 1, MaxTime, counter);
	sprite.pos.y = Easing::EaseInOutQuart(lastPosY + 100, lastPosY, MaxTime, counter);
}

