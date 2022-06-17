#pragma once
#include "Timer.h"
#include "Vector.h"

class EnergyItem;

class ItemEmitter
{
public:
	static ItemEmitter* GetInstance();
	static void DeleteInstance();

	void Initialize();
	void Update();

	void EmitEnergyItem(const Vector3& arg_pos);
	void EmitPanelItem(const Vector3& arg_pos);

private:
	ItemEmitter();
	~ItemEmitter();
	ItemEmitter(const ItemEmitter& another) = delete;
	ItemEmitter& operator=(const ItemEmitter& another) = delete;

	static ItemEmitter* instance;

	Timer* energyItemTimer;

	Vector3 GetRandomEmitPosition(const int arg_min, const int arg_max);
};