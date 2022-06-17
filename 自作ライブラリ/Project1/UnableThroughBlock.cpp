#include "UnableThroughBlock.h"
#include "OBJLoader.h"
#include "FieldPiece.h"

UnableThroughBlock::UnableThroughBlock(const Vector3& arg_position, const float arg_height, FieldPiece* arg_piece)
	:virtualityPlanePosition(arg_position),
	 preVirtualityPlanePosition(arg_position),
	 dir(arg_piece->GetPieceDirection()),
	 parentPiece(arg_piece)
{
	points = parentPiece->GetPoints();
	Create(OBJLoader::GetModel("fieldPiece"));

	Initialize();
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
	return virtualityPlanePosition;
}

PieceDirection UnableThroughBlock::GetPieceDirection() const
{
	return dir;
}

std::vector<Vector2>& UnableThroughBlock::GetPoints()
{
	return points;
}
