//#pragma once
//#include "Sprite.h"
//#include "TextureResource.h"
//#include "Vector.h"
//
//class Menu
//{
//public:
//	Menu();
//	void Initialize();
//	bool Update();
//	void Draw();
//	bool GetRestart() { return restartFlag; }
//	bool GetReturnTitle() { return returnTitle; }
//
//private:
//	//開いた最初の画面の処理
//	void BaseMenu();
//	//設定項目選択画面の処理
//	void Setting();
//	//感度設定処理
//	void SensitiveSetting();
//	//操作設定処理
//	void OperateSetting();
//	//画面設定処理
//	void ScreenSettingProcess();
//	//やり直し画面の処理
//	void RestartCheckProcess();
//	//タイトルに戻る画面の処理
//	void EndCheckProcess();
//	//各条件でのテクスチャ―パラメータを変更する
//	void SetTexParam();
//	//シーン遷移処理
//	void Migrate();
//	enum MenuState
//	{
//		Base,//一番大枠(最初の画面)
//		SettingSelect,//設定項目選択
//		PadSensitiveSetting,//padの感度設定
//		PadOperateSetting,//操作設定
//		ScreenSetting,//画面設定
//		RestartCheck,//やりなおし
//		EndCheck,//タイトルに戻る
//	};
//	MenuState menuState;
//	MenuState nextMenuState;
//	
//	int openCounter = 0;
//	int closeCounter = 0;
//	bool startClose = false;
//	
//	std::unique_ptr<Sprite> menuUI;
//	std::unique_ptr<Sprite> backTex;
//	std::unique_ptr<Sprite> restart;
//	std::unique_ptr<Sprite> end;
//	std::unique_ptr<Sprite> setting;
//	std::unique_ptr<Sprite> yes;
//	std::unique_ptr<Sprite> no;
//	std::unique_ptr<Sprite> endCheck;
//	std::unique_ptr<Sprite> restartCheck ;
//	std::unique_ptr<Sprite> sensitiveSetting;
//	std::unique_ptr<Sprite> operateSetting;
//	std::unique_ptr<Sprite> sensitiveSettingBar;
//	std::unique_ptr<Sprite> dot;
//	std::unique_ptr<Sprite> check;
//	std::unique_ptr<Sprite> check2;
//	std::unique_ptr<Sprite> jump;
//	std::unique_ptr<Sprite> cameraReset;
//	std::unique_ptr<Sprite> action;
//	std::unique_ptr<Sprite> upsideDown;
//	std::unique_ptr<Sprite> changeRun;
//	std::unique_ptr<Sprite> flipLeftRight;
//	std::unique_ptr<Sprite> jumpOperate ;
//	std::unique_ptr<Sprite> cameraResetOperate;
//	std::unique_ptr<Sprite> actionOperate;
//	std::unique_ptr<Sprite> changeRunOperate;
//	std::unique_ptr<Sprite> selectFrame; 
//	std::unique_ptr<Sprite> viewCollision;
//	std::unique_ptr<Sprite> screenSetting;
//	std::unique_ptr<Sprite> sSAO;
//	
//	bool operateChangeFlag = false;
//	
//	//Sprite* number = nullptr;
//
//	std::unordered_map<int, std::string> operateTexNames;
//	
//	int select = 0;
//	float easingScale;
//	Vector2 menuUIScale;
//	Vector2 backTexScale;
//	Vector2 restartScale;
//	Vector2 endScale;
//	Vector2 settingScale;
//	Vector2 sensitiveSettingScale;
//	Vector2	operateSettingScale;
//	Vector2 yesScale;
//	Vector2 noScale;
//	Vector2 dotPos;
//	Vector2 selectFramePos;
//	Vector2 screenSettingScale;
//	bool close = false;
//
//	//やりなおし、タイトルに戻るフラグ
//	bool returnTitle = false;
//	bool restartFlag = false;
//	std::unique_ptr <TextureResource> resource;
//
//	bool onSSAO;
//
//	float texAlpha;
//	//遷移用カウンターとフラグ
//	int migrateCounter;
//	bool migrate;
//};
//
