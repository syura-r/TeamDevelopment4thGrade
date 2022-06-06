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

	void SetUsePause(const bool& useFlag) { this->useFlag = useFlag; }
	bool GetUsePause() { return useFlag; }

	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	bool useFlag = false;

	enum SelectState
	{
		ToGame = 0,
		Restart = 1,
		ToTitle = 2,
	};
	SelectState selectState;
	//�I�����̌�
	const int selectMax = 3;

	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};

	//�Q�[���ɂ��ǂ�
	Sprite* sp_toGame = nullptr;
	Vector2 pos_toGame = {};

	//��蒼��
	Sprite* sp_restart = nullptr;
	Vector2 pos_restart = {};
	bool flag_restart = false;

	//�^�C�g���ɂ��ǂ�
	Sprite* sp_toTitle = nullptr;
	Vector2 pos_toTitle = {};
	bool flag_toTitle = false;

	//Config* sp_config = nullptr;
};
