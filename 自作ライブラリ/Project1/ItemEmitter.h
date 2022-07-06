#pragma once
#include "Timer.h"
#include "Vector.h"

class FieldPiece;
class EnergyItem;
enum class RankEnergyItem;

class ItemEmitter
{
public:
	static ItemEmitter* GetInstance();
	static void DeleteInstance();

	void Initialize();
	void Update();

	bool IsClushedWithRemainItem(FieldPiece* arg_piece);

	void EmitEnergyItem(const Vector3& arg_pos, const RankEnergyItem arg_rank, FieldPiece* arg_piece);
	void EmitPanelItem(const Vector3& arg_pos);
	Vector3 GetRandomEmitPosition(const int arg_min, const int arg_max);
	Vector3 GetEnergyItemEmitPosition();

private:
	ItemEmitter();
	~ItemEmitter();
	ItemEmitter(const ItemEmitter& another) = delete;
	ItemEmitter& operator=(const ItemEmitter& another) = delete;

	static ItemEmitter* instance;

	Timer* energyItemTimer;
};