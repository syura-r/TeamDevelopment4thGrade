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

	bool GetToGame() { return flag_toGame; }
	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//�I������I�ԏ���
	void Select();
	void SelectSub_Sound();
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
		Sound = 3,
	};
	SelectState selectState;
	//�I�����̌�
	const int selectMax = 4;

	enum SelectState_Sound
	{
		BGM = 0,
		SE = 1,
	};
	SelectState_Sound selectState_sound;
	//�I�����̌�
	const int selectMax_sound = 2;

	//X���W�̂��炷�
	static const int positionStepMax = 4;
	static float positions_X[positionStepMax];

	//�Q�[����ʂ��Â�
	Sprite* sp_back = nullptr;
	Vector2 pos_back = {};

	//�I�������₷��
	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};
	float alpha_base = 1.0f;
	bool isUP_alphaChange = false;

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
	bool flag_toGame = false;

	//��蒼��
	SelectSprite* restart = nullptr;
	bool flag_restart = false;

	//�^�C�g���ɂ��ǂ�
	SelectSprite* toTitle = nullptr;
	bool flag_toTitle = false;

	//���ʐݒ�
	SelectSprite* sound = nullptr;
	bool flag_sound = false;//BGM��SE�̐ݒ��W�J����
	//BGM
	SelectSprite* bgm = nullptr;
	Sprite* bar_bgm;
	Vector2 barPositionLeft_bgm;
	const Vector2 bar_scale = { 256.0f,5.0f };
	Sprite* circle_bgm;
	Vector2 circlePosition_bgm;
	//SE
	SelectSprite* se = nullptr;
	Sprite* bar_se;
	Vector2 barPositionLeft_se;
	Sprite* circle_se;
	Vector2 circlePosition_se;

	//�������
	Sprite* config_sp = nullptr;
};
