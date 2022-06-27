#pragma once
#include "Sprite.h"
#include "Vector.h"

class Pause
{
public:
	Pause();
	~Pause();

	void Initialize();
	void Update();
	void Draw();

	bool GetActivePause() { return activeFlag; }

	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//�I������I�ԏ���
	void Select();
	//����{�^������������̏���
	void Decision();

	//�`���X�V�������s����
	bool activeFlag = false;
	//
	static bool fadeFlag;


	enum SelectState
	{
		ToGame = 0,
		Restart = 1,
		ToTitle = 2,
	};
	SelectState selectState;
	//�I�����̌�
	const int selectMax = 3;

	//X���W�̂��炷�
	static const int positionStepMax = 3;
	static float positions_X[positionStepMax];

	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};

	//�I����1�ɕK�v�ȕϐ�
	struct SelectSprite
	{
		SelectSprite();
		~SelectSprite();
		void Initialize(const std::string& texName, const float posY);
		void Update();
		void Draw();

		//�����o���O����
		void PreMoveSetting();

		Sprite* sprite = nullptr;
		std::string texName = "";
		Vector2 pos = {};
		Vector2 prevPos = {};
		int step = 0;
		int easeTime = 0;
	};


	//�Q�[���ɂ��ǂ�
	SelectSprite* toGame = nullptr;

	//��蒼��
	SelectSprite* restart = nullptr;
	bool flag_restart = false;

	//�^�C�g���ɂ��ǂ�
	SelectSprite* toTitle = nullptr;
	bool flag_toTitle = false;

};
