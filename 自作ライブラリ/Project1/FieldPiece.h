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

	bool IsRidden(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius);

	static float GetSize();
	static float GetSidewaysLength();
	static float GetLowerTimeOffset();
	static float GetUpperTimeOffset();
	static float GetFullOffset();
	Vector3 GetVirtualityPlanePosition()const;
	static float GetWeight();
	PieceDirection GetPieceDirection()const;
	bool IsActive();
	void ChangeIsActive(const bool arg_isActive);
	bool IsBlockade();
	void ChangeIsBlockade(const bool arg_isBlockade);
	std::vector<Vector2>& GetPoints();

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
	bool isBlockade;

	std::vector<Vector2> points;
	static std::vector<Vector2> basePoints;

	void SetPoints();
	void ChangeColorForRidden();
};