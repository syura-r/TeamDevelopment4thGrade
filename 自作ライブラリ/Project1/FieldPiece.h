#pragma once
#include "Object.h"
#include "LocusDef.h"
#include "Timer.h"

enum class PieceDirection
{
	Lower,
	Upper,
};

class FieldPiece : public Object
{
public:
	FieldPiece(const Vector3& arg_position, const PieceDirection arg_direction);
	~FieldPiece();

	void Initialize()override;
	void Update()override;
	void Draw()override;

	static float GetSize();
	static float GetSidewaysLength();
	static float GetLowerTimeOffset();
	static float GetUpperTimeOffset();
	static float GetFullOffset();
	static float GetWeight();
	bool IsActive();

private:
	static const float SIZE;
	static const float SIDEWAYS_LENGTH;
	static const float LOWER_TIME_OFFSET;
	static const float UPPER_TIME_OFFSET;
	static const float FULL_OFFSET;
	//•½–Ê‚Ì‚Ü‚Ü‚Ìposition
	Vector3 virtualityPlanePosition;
	static const float WEIGHT;
	PieceDirection dir;
	bool isActive;

	std::vector<Vector2> points;
	static std::vector<Vector2> basePoints;

	void SetPoints();
};