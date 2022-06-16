#include "UnableThroughBlock.h"
#include "FieldPiece.h"

UnableThroughBlock::UnableThroughBlock(const Vector3& arg_position, const float arg_height, const FieldPiece* arg_piece)
{
}

UnableThroughBlock::~UnableThroughBlock()
{
}

void UnableThroughBlock::Initialize()
{
}

void UnableThroughBlock::Update()
{
}

void UnableThroughBlock::Draw()
{
}

Vector3 UnableThroughBlock::GetVirtualityPlanePosition() const
{
	return Vector3();
}

PieceDirection UnableThroughBlock::GetPieceDirection() const
{
	return PieceDirection();
}

std::vector<Vector2>& UnableThroughBlock::GetPoints()
{
	return points;
}
