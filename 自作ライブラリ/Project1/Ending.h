#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "NumberSprite.h"

class Ending :public Scene
{
public:
	Ending();
	~Ending()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;

private:
	void SelectMenu();

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;

	//���U���g
	Sprite* sp_result = nullptr;

	//�����Ɛ��l�̃Z�b�g
	struct ResultSet
	{
		ResultSet();
		~ResultSet();
		void Initialize(const float arg_num);
		void Update(const bool skipLook = true);
		void Draw(
			const std::string& arg_texName_text,
			const float arg_position_y,
			const Vector2& arg_scale);

		Sprite* sp_text = nullptr;
		NumberSprite* numSp_number = nullptr;
		float num = 0.0f;
		float drawNum = 0.0f;
		bool isCountEnd = false;
	};

	//�݌v�X�R�A
	ResultSet* totalScore = nullptr;
	//�؂蔲�����p�l����
	ResultSet* cutPanel = nullptr;
	//���Ƃ����G�̐�
	ResultSet* dropEnemy = nullptr;

	//�I������
	Sprite* sp_select = nullptr;
	Vector2 pos_select = {};

	//���X�^�[�g
	Sprite* sp_restart = nullptr;
	const Vector2 pos_restart = { 640, 864 };

	//�^�C�g��
	Sprite* sp_title = nullptr;
	const Vector2 pos_title = { 1280, 864 };

};

