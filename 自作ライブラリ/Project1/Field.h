#pragma once
#include "Object.h"
#include "BoxCollider.h"
#include "Quaternion.h"
#include "LocusDef.h"
#include "Timer.h"

class FieldPiece;
class UnableThroughBlock;
class UnableThroughEdge;

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

	//�}�`�����ɂ���ČX���Ƃ�
	void AddInfluence(const LocusFieldInfluence& arg_inf);
	void ResetInfluences();
	int CutPanel(std::vector<Vector2>& arg_vecPos);
	void ReviveGottenPanel(FieldPiece* arg_piece);
	void StartFallingBlock();

	CuttingInfo* GetCuttingInfo(Object* arg_pObject);
	FieldPiece* IsRideGottenPanel(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius);

	static float GetRadius();
	static std::vector<Vector2>& GetEdges();
	Vector3 GetAngleTilt()const;
	Vector3 GetTilt();
	float GetDepthMagnitude()const;	
	std::vector<FieldPiece*>& GetGottenPieces();

private:
	//�L��
	static const float RADIUS;
	static const int PIECE_LAYER_NUM;
	static std::vector<Vector2> edges;
	//�X����\��
	float depthMagnitude;
	const float MAX_DEPTH_MAGNITUDE = 10.0f;
	const float DEGREE_COEFFICIENT = 3.0f;
	Vector2 tiltDirection;
	//�ǂ̕����ɌX���Ă��邩
	Vector3 angleTilt;
	Vector3 localYvec;
	//�}�`�̏ꏊ�Əd��
	std::vector<LocusFieldInfluence> influences;
	//�p�l���̔z��
	std::vector<std::vector<FieldPiece*>> pieces;
	std::vector<FieldPiece*> gottenPieces;
	//Player���A�N�V��������p�l��	
	std::unordered_map<Object*, CuttingInfo> infos;

	//�^�C���A�b�v���p
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

	//�[�_�̗p��
	void SetEdges();
	//FieldPiece����
	void CreatePieces();
	//�X���v�Z
	void CalcTilt();
	float GetMultiplyingFactor(const float arg_length);
	//Player���A�N�V��������p�l���f��p
	void DecideAllRidingPiece();
	void DecideAllCuttingStartPos();
	void DecideAllCuttingAngle();
	void DecideRidingPiece(Object* arg_obj, const Vector3& arg_pos);
	void DecideCuttingStartPos(Object* arg_obj, const Vector3& arg_pos, const Vector3& arg_dir);
	void DecideCuttingAngle(Object* arg_obj);
	//
	void FallingBlock();
};