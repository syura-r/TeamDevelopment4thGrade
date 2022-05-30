#include "Menu.h"

#include "Easing.h"
#include "Input.h"
#include "Texture.h"
#include"DirectXLib.h"
#include "GameSettingParam.h"
#include "Input.h"
#include "PipelineState.h"
int SettingParam::padSensitive = 3;
int SettingParam::reverseX = 1;
int SettingParam::reverseY = 1;

int SettingParam::jumpButton = XINPUT_GAMEPAD_A;
int SettingParam::airSlideButton = XINPUT_GAMEPAD_B;
int SettingParam::runButton = XINPUT_GAMEPAD_LEFT_SHOULDER;
int SettingParam::resetButton = XINPUT_GAMEPAD_RIGHT_SHOULDER;
bool SettingParam::viewCollision = true;

bool SettingParam::onSSAO = false;

Menu::Menu()
{
//スプライトの生成
	menuUI = std::make_unique<Sprite>();
	backTex = std::make_unique<Sprite>();
	restart = std::make_unique<Sprite>();
	end = std::make_unique<Sprite>();
	setting = std::make_unique<Sprite>();
	yes = std::make_unique<Sprite>();
	no = std::make_unique<Sprite>();
	endCheck = std::make_unique<Sprite>();
	restartCheck = std::make_unique<Sprite>();
	sensitiveSetting = std::make_unique<Sprite>();
	operateSetting = std::make_unique<Sprite>();
	sensitiveSettingBar = std::make_unique<Sprite>();
	dot = std::make_unique<Sprite>();
	check = std::make_unique<Sprite>();
	check2 = std::make_unique<Sprite>();
	jump = std::make_unique<Sprite>();
	cameraReset = std::make_unique<Sprite>();
	action = std::make_unique<Sprite>();
	upsideDown = std::make_unique<Sprite>();
	changeRun = std::make_unique<Sprite>();
	flipLeftRight = std::make_unique<Sprite>();
	jumpOperate = std::make_unique<Sprite>();
	cameraResetOperate = std::make_unique<Sprite>();
	actionOperate = std::make_unique<Sprite>();
	changeRunOperate = std::make_unique<Sprite>();
	selectFrame = std::make_unique<Sprite>();
	viewCollision = std::make_unique<Sprite>();
	screenSetting = std::make_unique<Sprite>();
	sSAO = std::make_unique<Sprite>();
	
	//number = new Sprite();
//操作変更用のボタンと画像名を設定
	operateTexNames[XINPUT_GAMEPAD_LEFT_THUMB] = "LT";
	operateTexNames[XINPUT_GAMEPAD_RIGHT_THUMB] = "RT";
	operateTexNames[XINPUT_GAMEPAD_LEFT_SHOULDER] = "LB";
	operateTexNames[XINPUT_GAMEPAD_RIGHT_SHOULDER] = "RB";
	operateTexNames[XINPUT_GAMEPAD_A] = "a";
	operateTexNames[XINPUT_GAMEPAD_B] = "b";
	operateTexNames[XINPUT_GAMEPAD_X] = "x";
	operateTexNames[XINPUT_GAMEPAD_Y] = "y";
//メニュー画面全体の画像とSRVの生成
	resource.reset(new TextureResource("Menu", { 960,540 }));
}

void Menu::Initialize()
{
	easingScale = 0;
	openCounter = 0;
	menuUIScale = { 0,0 };
	backTexScale = { 1,1 };
	restartScale = { 1,1 };
	endScale = { 1,1 };
	settingScale = { 1.5f,1.5f };
	sensitiveSettingScale = { 1.5f,1.5f };
	operateSettingScale = { 1,1 };
	yesScale = { 1.5f,1.5f };
	noScale = { 1,1 };
	screenSettingScale = { 1,1 };
	select = 0;
	close = false;
	startClose = false;
	closeCounter = 0;
	menuState = Base;
	dotPos = { 480 - 228*1.5f,300 - 20*1.5f };
	selectFramePos = {};
	operateChangeFlag = false;
	onSSAO = SettingParam::onSSAO;
	SettingParam::onSSAO = false;
	restartFlag = false;
	returnTitle = false;
	texAlpha = 1;
	migrateCounter = 0;
	migrate = false;
}

bool Menu::Update()
{
//表示状態
	if (openCounter > 30)
	{
		//クローズ時の縮小処理
		if (startClose)
		{
			if (closeCounter > 30)
			{
				close = true;
				SettingParam::onSSAO = onSSAO;
			}
			easingScale = Easing::EaseInOutQuart(1.0f, 0.0f, 30.0f, (float)closeCounter);
			menuUIScale = { easingScale,easingScale };
			closeCounter++;
		}
		else
		{
			//遷移時は遷移処理のみで操作を受け付けない
			if (migrate)
			{
				Migrate();
				return false;
			}
			//メニュー選択画面の処理（メニュー本体）
			switch (menuState)
			{
			case Base:
				BaseMenu();
				break;
				
			case SettingSelect:
				Setting();
				break;
				
			case PadOperateSetting:
				OperateSetting();
				break;
				
			case PadSensitiveSetting:
				SensitiveSetting();
				break;
				
			case ScreenSetting:
				ScreenSettingProcess();
				break;
				
			case RestartCheck:
				RestartCheckProcess();
				break;
				
			case EndCheck:
				EndCheckProcess();
				break;
				
			default:
				break;
			}
			//どの画面でもメニューボタンが押されたら閉じる
			if (Input::TriggerKey(DIK_ESCAPE)||Input::TriggerPadButton(XINPUT_GAMEPAD_START))
				startClose = true;
		}
	}
//メニュー開いたときの拡大処理
	else
	{
		easingScale = Easing::EaseInOutQuart(0.0f, 1.0f, 30.0f, (float)openCounter);
		menuUIScale = { easingScale,easingScale };
		openCounter++;
	}
	return close;
}
void Menu::Draw()
{
	//リソースバリアの変更とレンダーターゲットをメニュー用のテクスチャに変更
	resource->PreDraw();
	backTex->DrawSprite("menuUI", { 480,270 }, 0, backTexScale);
	resource->DepthClear();
	//各画面状態に合わせて描画が内容を変更
	switch (menuState)
	{
	case Base:
		setting->DrawSprite("setting", { 480,104 }, 0, settingScale, { 1,1,1,texAlpha });
		restart->DrawSprite("restart", { 480,270 }, 0, restartScale, { 1,1,1,texAlpha });
		end->DrawSprite("end", { 480,436 }, 0, endScale, { 1,1,1,texAlpha });
		break;

	case SettingSelect:
		sensitiveSetting->DrawSprite("sensitiveSetting", { 480,104 }, 0, sensitiveSettingScale, { 1,1,1,texAlpha });
		operateSetting->DrawSprite("operateSetting", { 480,270 }, 0, operateSettingScale, { 1,1,1,texAlpha });
		screenSetting->DrawSprite("screenSetting", { 480,436 }, 0, screenSettingScale, { 1,1,1,texAlpha });
		break;

	case PadOperateSetting:
		operateSetting->DrawSprite("operateSetting", { 480,105 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		upsideDown->DrawSprite("upsideDown", { 295,200 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		//ONの時のみチェックマークを描画
		if (SettingParam::reverseY == -1)
			check->DrawSprite("check", { 403,196 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		flipLeftRight->DrawSprite("flipLeftRight", { 655,200 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		
		//ONの時のみチェックマークを描画
		if (SettingParam::reverseX == -1)
			check2->DrawSprite("check", { 763,196 }, 0, { 1,1 }, { 1,1,1,texAlpha });

		jump->DrawSprite("jump", { 240,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		jumpOperate->DrawSprite(operateTexNames[SettingParam::jumpButton], { 400,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		cameraReset->DrawSprite("cameraReset", { 620,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		cameraResetOperate->DrawSprite(operateTexNames[SettingParam::resetButton], { 830,300 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		action->DrawSprite("action", { 240,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		actionOperate->DrawSprite(operateTexNames[SettingParam::airSlideButton], { 400,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		changeRun->DrawSprite("changeRun", { 620,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		changeRunOperate->DrawSprite(operateTexNames[SettingParam::runButton], { 830,400 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		selectFrame->DrawSprite("selectFrame", selectFramePos, 0, { 1,1 }, { 1,1,1,texAlpha });
		break;

	case PadSensitiveSetting:
		sensitiveSetting->DrawSprite("sensitiveSetting", { 480,130 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		dot->DrawSprite("dot", dotPos, 0, { 1,1 }, { 1,1,1,texAlpha });
		sensitiveSettingBar->DrawSprite("sensitiveSettingBar", { 480,300 }, 0, { 1.5f,1.5f }, { 1,1,1,texAlpha });

		break;
		
	case ScreenSetting:
		screenSetting->DrawSprite("screenSetting", { 480,105 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		viewCollision->DrawSprite("viewCollision", { 480,240 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		sSAO->DrawSprite("SSAO", { 614,390 }, 0, { 1,1 }, { 1,1,1,texAlpha });

		//ONの時のみチェックマークを描画
		if (SettingParam::viewCollision)
			check->DrawSprite("check", { 730,240 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		//ONの時のみチェックマークを描画
		if (onSSAO)
			check2->DrawSprite("check", { 730,390 }, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		selectFrame->DrawSprite("selectFrame", selectFramePos, 0, { 1.2f,1.2f }, { 1,1,1,texAlpha });
		break;
		
	case RestartCheck:
		restartCheck->DrawSprite("restartCheck", { 480,150 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		yes->DrawSprite("yes", { 320,380 }, 0, yesScale, { 1,1,1,texAlpha });
		no->DrawSprite("no", { 640,380 }, 0, noScale, { 1,1,1,texAlpha });
		break;

	case EndCheck:
		endCheck->DrawSprite("endCheck", { 480,150 }, 0, { 1,1 }, { 1,1,1,texAlpha });
		yes->DrawSprite("yes", { 320,380 }, 0, yesScale, { 1,1,1,texAlpha });
		no->DrawSprite("no", { 640,380 }, 0, noScale, { 1,1,1,texAlpha });
		break;

	default:
		break;
	}
	//リソースバリアを戻す
	resource->PostDraw();
	//レンダリングされたメニュー画像を描画
	menuUI->DrawSprite("Menu" + std::to_string(DirectXLib::GetInstance()->GetBbIndex()), { 960,540 }, 0, menuUIScale, { 1,1,1,1 }, { 0.5f,0.5f },"NoAlphaToCoverageSprite");
}

void Menu::BaseMenu()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 2)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			nextMenuState = SettingSelect;
			break;
		case 1:
			nextMenuState = RestartCheck;
			break;
		case 2:
			nextMenuState = EndCheck;
			break;
		default:
			return;
		}
		migrate = true;
		SetTexParam();
	}

	if(Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		startClose = true;
	}
}

void Menu::Setting()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 2)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			nextMenuState = PadSensitiveSetting;
			break;
		case 1:
			nextMenuState = PadOperateSetting;
			break;
		case 2:
			nextMenuState = ScreenSetting;
			break;
		default:
			return;
		}
		migrate = true;
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::SensitiveSetting()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))

	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (SettingParam::padSensitive < 10)
				SettingParam::padSensitive++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (SettingParam::padSensitive > 1)
				SettingParam::padSensitive--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		nextMenuState = SettingSelect;
		migrate = true;
		SetTexParam();
	}
}

void Menu::OperateSetting()
{
	if (operateChangeFlag)
	{
		//何かしらのPADのボタンが押されたら代入される
		int button = Input::CheckAnyPadButton();

		//押されたボタンが割り当て可能範囲内のボタンだったら
		if (button == XINPUT_GAMEPAD_LEFT_THUMB||
			button == XINPUT_GAMEPAD_RIGHT_THUMB||
			button == XINPUT_GAMEPAD_LEFT_SHOULDER||
			button == XINPUT_GAMEPAD_RIGHT_SHOULDER||
			button == XINPUT_GAMEPAD_A||
			button == XINPUT_GAMEPAD_B||
			button == XINPUT_GAMEPAD_X||
			button == XINPUT_GAMEPAD_Y)
		{
			//変更対象によって場合分け＋ボタンの変更＋変更後のボタンが既に他の操作に割り当て済みの場合ボタンを入れ替える
			switch (select)
			{
			case 2:
				if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::jumpButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::jumpButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::jumpButton;

				SettingParam::jumpButton = button;
				break;
			case 3:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::resetButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::resetButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::resetButton;

				SettingParam::resetButton = button;
				break;
			case 4:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::airSlideButton;

				else if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::airSlideButton;

				else if (SettingParam::runButton == button) SettingParam::runButton = SettingParam::airSlideButton;

				SettingParam::airSlideButton = button;
				break;
			case 5:
				if (SettingParam::jumpButton == button) SettingParam::jumpButton = SettingParam::runButton;

				else if (SettingParam::resetButton == button) SettingParam::resetButton = SettingParam::runButton;

				else if (SettingParam::airSlideButton == button) SettingParam::airSlideButton = SettingParam::runButton;

				SettingParam::runButton = button;
				break;
			default:
				return;
			}
			operateChangeFlag = false;
		}
		return;
	}
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)||
		Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D) || Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 5)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select < 5)
			{
				select += 2;
				if (select > 5)
					select = 5;
			}
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
			{
				select -= 2;
				if (select < 0)
					select = 0;
			}
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			SettingParam::reverseY = SettingParam::reverseY == 1 ? -1 : 1;
			break;
		case 1:
			SettingParam::reverseX = SettingParam::reverseX == 1 ? -1 : 1;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			operateChangeFlag = true;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		nextMenuState = SettingSelect;
		migrate = true;
		SetTexParam();
	}

}

void Menu::ScreenSettingProcess()
{
	if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::TriggerKey(DIK_W) || Input::TriggerKey(DIK_S))
	{
		if (Input::TriggerPadLStickDown() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN) || Input::TriggerKey(DIK_S))
		{
			if (select <1)
				select++;
		}
		if (Input::TriggerPadLStickUp() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) || Input::TriggerKey(DIK_W))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			SettingParam::viewCollision = SettingParam::viewCollision == true ? false : true;
			break;
		case 1: 
			onSSAO = onSSAO == true ? false : true;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = SettingSelect;
		SetTexParam();
	}

}

void Menu::RestartCheckProcess()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 1)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			restartFlag = true;
			startClose = true;
			break;
		case 1:
			migrate = true;
			nextMenuState = Base;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::EndCheckProcess()
{
	if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::TriggerKey(DIK_A) || Input::TriggerKey(DIK_D))
	{
		if (Input::TriggerPadLStickRight() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT) || Input::TriggerKey(DIK_D))
		{
			if (select < 1)
				select++;
		}
		if (Input::TriggerPadLStickLeft() || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) || Input::TriggerKey(DIK_A))
		{
			if (select > 0)
				select--;
		}
		SetTexParam();
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_SPACE))
	{
		switch (select)
		{
		case 0:
			returnTitle = true;
			startClose = true;
			break;
		case 1:
			migrate = true;
			nextMenuState = Base;
			break;
		default:
			return;
		}
		SetTexParam();
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_Z))
	{
		migrate = true;
		nextMenuState = Base;
		SetTexParam();
	}
}

void Menu::SetTexParam()
{
	switch (menuState)
	{
	case Base:
		switch (select)
		{
		case 0:
			restartScale = { 1,1 };
			endScale = { 1,1 };
			settingScale = { 1.5f,1.5f };
			break;
		case 1:
			restartScale = { 1.5f,1.5f };
			endScale = { 1,1 };
			settingScale = { 1,1 };
			break;
		case 2:
			restartScale = { 1,1 };
			endScale = { 1.5f,1.5f };
			settingScale = { 1,1 };
			break;
		default:
			break;
		}

		break;

	case SettingSelect:
		switch (select)
		{
		case 0:
			sensitiveSettingScale = { 1.5f,1.5f };
			operateSettingScale = { 1,1 };
			screenSettingScale = { 1,1 };
			break;
		case 1:
			operateSettingScale = { 1.5f,1.5f };
			sensitiveSettingScale = { 1,1 };
			screenSettingScale = { 1,1 };
			break;
		case 2:
			operateSettingScale = { 1,1 };
			sensitiveSettingScale = { 1,1 };
			screenSettingScale = { 1.5f,1.5f };
			break;
		default:
			break;
		}
		break;

	case PadOperateSetting:
		switch (select)
		{
		case 0:
			selectFramePos = { 403,200 };
			break;
		case 1:
			selectFramePos = { 763,200 };
			break;
		case 2:
			selectFramePos = { 400,300 };
			break;
		case 3:
			selectFramePos = { 830,300 };
			break;
		case 4:
			selectFramePos = { 400,400 };
			break;
		case 5:
			selectFramePos = { 830,400 };
			break;

		default:
			break;
		}

		break;

	case PadSensitiveSetting:
		dotPos = { (480 - 228 * 1.5f) + 49 * 1.5f * (SettingParam::padSensitive - 1),300 - 20 * 1.5f };
		break;
	case ScreenSetting:
		switch (select)
		{
		case 0:
			selectFramePos = { 718,240 };
			break;
		case 1:
			selectFramePos = { 718,390 };
			break;
		default:
			break;
		}

		break;
	case RestartCheck:
		switch (select)
		{
		case 0:
			yesScale = { 1.5f,1.5f };
			noScale = { 1,1 };
			break;
		case 1:
			noScale = { 1.5f,1.5f };
			yesScale = { 1,1 };
			break;
		default:
			break;
		}
		break;

	case EndCheck:
		switch (select)
		{
		case 0:
			yesScale = { 1.5f,1.5f };
			noScale = { 1,1 };
			break;
		case 1:
			noScale = { 1.5f,1.5f };
			yesScale = { 1,1 };
			break;
		default:
			break;
		}

		break;

	default:
		break;
	}

}

void Menu::Migrate()
{
	if(migrateCounter<=30)
	{
		texAlpha = Easing::EaseInOutQuart(1.0f, 0.0f, 30.0f, (float)migrateCounter);
	}
	else if(migrateCounter<=60)
	{
		texAlpha = Easing::EaseInOutQuart(0.0f, 1.0f, 30.0f, (float)migrateCounter-30);
	}
	else
	{
		migrate = false;
		migrateCounter = 0;
		return;
	}
	if(migrateCounter == 30)
	{
		menuState = nextMenuState;
		select = 0;
		SetTexParam();
	}
	migrateCounter++;
}
