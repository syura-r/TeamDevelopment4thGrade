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

	//図形完成によって傾くとき
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();
	int CutPanel(std::vector<Vector2>& arg_vecPos);

	FieldPiece* GetPlayerRidingPiece();
	Vector3 GetPlayerCuttingStartPos();
	float GetPlayerCuttingAngle();

	static std::vector<Vector2>& GetEdges();
	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;	

private:
	//広さ
	const float RADIUS = 45.0f;
	static const int PIECE_LAYER_NUM;
	static std::vector<Vector2> edges;
	//傾きを表す
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 5.0f;
	Vector2 tiltDirection;
	//どの方向に傾いているか
	Vector3 angleTilt;
	Vector3 localYvec;
	//図形の場所と重さ
	std::vector<LocusFieldInfluence> influences;

	std::vector<std::vector<FieldPiece*>> pieces;
	FieldPiece* playerRidingPiece;
	int playerCuttingStartPosNum;
	Vector3 playerCuttingStartPos;
	float playerCuttingAngle;

	//端点の用意
	void SetEdges();
	//FieldPiece生成
	void CreatePieces();
	//傾き計算
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);

	void DecidePlayerRidingPiece();
	void DecidePlayerCuttingStartPos();
	void DecidePlayerCuttingAngle();
};