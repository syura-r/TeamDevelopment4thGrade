#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"
#include "LocusDef.h"

class FieldPiece;

class Field : public Object
{
public:
	Field();
	~Field();

	void Initialize()override;
	void Update()override;
	void DrawReady()override;
	void Draw()override;

	//}Œ`Š®¬‚É‚æ‚Á‚ÄŒX‚­‚Æ‚«
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();

	static std::vector<Vector2>& GetEdges();
	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;	

private:
	//L‚³
	const float RADIUS = 45.0f;
	static const int PIECE_LAYER_NUM;
	static std::vector<Vector2> edges;
	//ŒX‚«‚ğ•\‚·
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//‚Ç‚Ì•ûŒü‚ÉŒX‚¢‚Ä‚¢‚é‚©
	Vector3 angleTilt;
	Vector3 localYvec;
	//}Œ`‚ÌêŠ‚Æd‚³
	std::vector<LocusFieldInfluence> influences;

	std::vector<FieldPiece*> pieces;

	//’[“_‚Ì—pˆÓ
	void SetEdges();
	//FieldPiece¶¬
	void CreatePieces();
	//ŒX‚«ŒvZ
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
};