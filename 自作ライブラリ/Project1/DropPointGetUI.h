#pragma once
#include "Sprite3D.h"

class DropPointGetUI
{
public:
	DropPointGetUI(Vector3& parentPosition, const std::string& parentName);
	~DropPointGetUI();

	void Initialize();
	void Update();
	void Draw();

	//���Đ����������Ƃ��Ă�
	void PointGet();

private:
	static int enemyCount;

	//�e�̍��W
	Vector3& parentPosition;
	std::string texName;
	const float addPositionY = 3.0f;
	const float scale = 0.8f;

	struct OneUI
	{
		void Initialize(const Vector3& parentPosition, const float addPositionY);

		Sprite3D* sprite = nullptr;
		Vector3 position;
		float alpha;
		bool isActive = false;
		int moveTimer = 0;
	};

	std::vector<OneUI*> ui;
};
