#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"
#include "LocusDef.h"
#include "Timer.h"

class FieldPiece;
class UnableThroughBlock;
class UnableThroughEdge;
class PanelCutLocus;
class BaseGameActor;
enum class ObjectRegistType;

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
	int CutPanel(PanelCutLocus* arg_locus, int& arg_bonusCount, BaseGameActor* arg_target);
	void ReviveGottenPanel(FieldPiece* arg_piece);
	void StartFallingBlock();

	void DecideCuttingInfo(Object* arg_object, const Vector3& arg_pos, const Vector3& arg_dir);
	CuttingInfo* GetCuttingInfo(Object* arg_pObject);
	FieldPiece* IsRideGottenPanel(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius);
	FieldPiece* GetRespawnPiece(const ObjectRegistType arg_type);
	void ChangeIsCutableWithAround(FieldPiece* arg_piece, const bool arg_flag);
	std::vector<FieldPiece*> GetAroundPiece(FieldPiece* arg_piece, const int colNum, const int rowNum);
	FieldPiece* GetRandomActivePiece();
	bool IsNewFeverPlayer();
	float GetLengthToFieldBorder(const int arg_index, const Vector3& arg_pos);
	Vector3 GetFieldBorderNormal(const int arg_index);
	void GetPiecesWithinSawRange(PanelCutLocus* arg_locus, std::vector<FieldPiece*>& arg_vecPieces);

	static float GetRadius();
	static std::vector<Vector2>& GetEdges();
	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;	
	std::vector<FieldPiece*>& GetGottenPieces();
	int GetFeverNolma();
	int GetGottenCount();

private:
	//広さ
	static const float RADIUS;
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
	std::unordered_map<Object*, CuttingInfo> infos;

	//タイムアップ時用
	bool isFallingBlock;
	int fallingLayerNum;
	int innerRemainLayerNum;
	int topLayerPieceNum;
	int onesidePieceNum;	
	int fallingBlockCount;
	int fallingBlockCountMax;
	Timer* fallIntervalTimer;
	const int INTERVAL_CREATE = 0.1f * 60;
	const int INTERVAL_WAIT = 5 * 60;
	UnableThroughEdge* lastTimeEdge;
	std::vector<UnableThroughBlock*> blocks;

	//ボーマスパネル再設定間隔
	Timer* setBonusPanelTimer;
	const unsigned int setBonusPanelInterval = 10 * 60;
	int bonusPanelCount;

	//フィーバー管理用
	int feverNolma;
	int gottenCount;
	int previousGottenCount;

	//端点の用意
	void SetEdges();
	//FieldPiece生成
	void CreatePieces();
	//傾き計算
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
	//Playerがアクションするパネル断定用
	void DecideAllRidingPiece();
	void DecideAllCuttingStartPos();
	void DecideAllCuttingAngle();
	void DecideRidingPiece(Object* arg_obj, const Vector3& arg_pos);
	void DecideCuttingStartPos(Object* arg_obj, const Vector3& arg_pos, const Vector3& arg_dir);
	void DecideCuttingAngle(Object* arg_obj);
	//タイムアップ時ブロックを落とす
	void FallingBlock();
	//ボーナスパネルの設定
	void SetBonusPanel();
};