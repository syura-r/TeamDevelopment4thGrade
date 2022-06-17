#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"
#include "LocusDef.h"

class FieldPiece;

struct CuttingInfo
{
	FieldPiece* ridingPiece;
	int cuttingStartPosNum;
	Vector3 cuttingStartPos;
	float cuttingAngle;

	CuttingInfo(FieldPiece* arg_piece = nullptr, const int arg_posNum = 0, const Vector3& arg_startPos = Vector3(), const float arg_angle = 0)
		:ridingPiece(arg_piece),
		 cuttingStartPosNum(arg_posNum),
		 cuttingStartPos(arg_startPos),
		 cuttingAngle(arg_angle)
	{
	}
};

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
	void ReviveGottenPanel(FieldPiece* arg_piece);

	FieldPiece* GetPlayerRidingPiece();
	Vector3 GetPlayerCuttingStartPos();
	float GetPlayerCuttingAngle();
	FieldPiece* IsRideGottenPanel(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius);

	static std::vector<Vector2>& GetEdges();
	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;	
	std::vector<FieldPiece*>& GetGottenPieces();

private:
	//広さ
	const float RADIUS = 50.0f;
	static const int PIECE_LAYER_NUM;
	static std::vector<Vector2> edges;
	//傾きを表す
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 3.0f;
	Vector2 tiltDirection;
	//どの方向に傾いているか
	Vector3 angleTilt;
	Vector3 localYvec;
	//図形の場所と重さ
	std::vector<LocusFieldInfluence> influences;
	//パネルの配列
	std::vector<std::vector<FieldPiece*>> pieces;
	std::vector<FieldPiece*> gottenPieces;
	//Playerがアクションするパネル
	CuttingInfo info;

	//端点の用意
	void SetEdges();
	//FieldPiece生成
	void CreatePieces();
	//傾き計算
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
	//Playerがアクションするパネル断定用
	void DecidePlayerRidingPiece();
	void DecidePlayerCuttingStartPos();
	void DecidePlayerCuttingAngle();
};