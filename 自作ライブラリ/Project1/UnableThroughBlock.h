#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

enum class PieceDirection;
class FieldPiece;

class UnableThroughBlock : public Object
{
public:
	UnableThroughBlock(const Vector3& arg_position, const float arg_height, const FieldPiece* arg_piece);
	~UnableThroughBlock();

	void Initialize()override;
	void Update()override;
	void Draw()override;	
	
	Vector3 GetVirtualityPlanePosition()const;	
	PieceDirection GetPieceDirection()const;
	std::vector<Vector2>& GetPoints();

private:	
	//•½–Ê‚Ì‚Ü‚Ü‚Ìposition
	Vector3 virtualityPlanePosition;
	Vector3 preVirtualityPlanePosition;	
	PieceDirection dir;	
	std::vector<Vector2> points;	
};