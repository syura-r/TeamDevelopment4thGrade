#pragma once
#include "Scene.h"
#include"Texture.h"
#include"Alpha.h"
#include "NumberSprite.h"
#include "Object3D.h"
#include "InGameCamera.h"
#include"LightGroup.h"
#include "Stadium.h"

class Ending :public Scene
{
public:
	Ending();
	~Ending()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;

public:
	static int score_player;
	static int score_enemy_red;
	static int score_enemy_green;

private:
	std::unique_ptr<InGameCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	void SelectMenu();
	//�_��
	void FlashMenu();

	enum SelectState
	{
		ToTitle = 0,
		Restart = 1,
	};
	SelectState selectState;

	//
	enum ActorTag
	{
		PLAYER,
		ENEMY_RED,
		ENEMY_GREEN,
	};

	struct ResultSet
	{
		ResultSet(const ActorTag& arg_tag);
		~ResultSet();
		void Initialize(const int arg_score, const float arg_positionX_3d, const float arg_positionX_2d);
		void Update(const int arg_topScore);
		void Draw();

		//�@�̃��f���֘A
		Object3D* object = nullptr;
		Vector3 position = { 0.0f, -7.0f, 10.0f };
		Vector3 scale = { 1,1,1 };
		Vector3 rotation = { 0,180,0 };
		Vector4 color = { 1,1,1,1 };
		//����
		Object3D* obj_crown;
		Vector3 pos_crown = { position.x, 8.0f, position.z };
		Vector3 scale_crown = { 1,1,1 };
		Vector3 rotation_crown = { -15,0,0 };
		Vector4 color_crown = { 1,1,1,1 };
		bool isTop = false;//���Đ�����ԑ�����
		bool drawCrown = false;//���Đ�����ԑ�����
		//���Đ�
		NumberSprite* numberSprite = nullptr;
		//�Q�[�W
		Sprite* sp_gauge = nullptr;
		float scaleY_gauge = 0.0f;
		float scaleY_gauge_draw = 0.0f;
		float score = 0.0f;
		float score_draw = 0.0f;
		float positionX_2d;

		ActorTag tag;
	};
	static const int enemyCount = 2;//�G�̐�
	ResultSet* set[enemyCount + 1];

	//�������ǂ�ɕt���邩
	void TopSearch();
	//�Q�[�W�̃T�C�Y���w��
	void GaugeSize();

	const float positions_3d[enemyCount + 1] = { -9.7f,0.0f,9.7f };
	const float positions_2d[enemyCount + 1] = { 500,960,1420 };

	//�����𑝂₷���p
	int topScore = 0;

	//�X�L�b�v
	void MotionSkip();
	bool isSkipOrFinish = false;

	//�I������
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};
	Vector2 scale_select = {};
	float alpha_select = 1.0f;
	bool isUP_alphaChange = false;

	const Vector2 scaleBig_select = { 1.5f,1.5f };
	const Vector2 scaleSmall_select = { 1.0f,1.0f };

	//�^�C�g��
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 640, 970 };
	Vector2 scale_title = { 1.0f,1.0f };
	float alpha_title = 1.0f;

	//���X�^�[�g
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 1280, 970 };
	Vector2 scale_restart = { 1.0f,1.0f };
	float alpha_restart = 1.0f;

	Stadium* stadium = nullptr;
};

