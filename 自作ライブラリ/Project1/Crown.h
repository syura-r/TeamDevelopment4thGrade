#pragma once
#include "Object3D.h"
#include "Vector.h"

class Crown
{
public:
	Crown();
	~Crown();

	void Initialize(const float arg_addPositionY);
	void Update(const int ranking, const Vector3& arg_position);
	void Draw();


private:
	Object3D* object = nullptr;
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 scale = { 1.5f, 1.5f, 1.5f };
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	Vector4 color = { 1, 1, 1, 1 };

	bool isTop = false;
	float addPositionY;
	float addPositionY_base;

	void MovePosition();

	const float addPositionY_up = 1.0f;
	const float addPositionY_down = -1.0f;
	bool isPositionUp;
	int positionUpEasingCount;
};

