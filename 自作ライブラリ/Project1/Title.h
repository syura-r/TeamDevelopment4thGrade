#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "TitleText.h"
#include "Sprite.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include"LightGroup.h"

class Title :
	public Scene
{
public:
	Title();
	~Title()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;

private:
	//パネル敷き詰め
	void PanelPadding();

	//遷移関連
	//パネル飛び出し
	bool PopUpPanel();
	//カメラ前進
	bool ZoomIn();

	std::unique_ptr<DebugCamera> camera;
	float cameraDistance;
	std::unique_ptr<LightGroup> lightGroup;

	Sprite* titleLogo = nullptr;
	Sprite* titleStart = nullptr;

	struct Panel
	{
		Panel();
		~Panel();

		void Initialize();

		Object3D* object = nullptr;
		Vector3 position = { 0,0,0 };
		Vector3 scale = { 1,1,1 };
		Vector3 rotation = { 0,0,0 };
		Vector4 color = { 1, 1, 1, 1 };
	};

	static const int panelsNum_X = 5;
	static const int panelsNum_Y = 3;
	static const int panelsNum_ALL = panelsNum_X * panelsNum_Y * 2;
	Panel* panels[panelsNum_ALL];

	//シーン遷移開始
	bool isSceneChange = false;
};
