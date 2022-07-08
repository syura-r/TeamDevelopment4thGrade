#include "Field.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "LocusDef.h"
#include "ActorManager.h"
#include "Player.h"
#include "StandardEnemy.h"
#include "FieldPiece.h"
#include "PanelCutLocus.h"
#include "Input.h"
#include "Easing.h"
#include "UnableThroughBlock.h"
#include "UnableThroughEdge.h"
#include "ItemEmitter.h"
#include "EnergyItem.h"
#include "ObjectRegistType.h"
#include "IActionState.h"

const int Field::PIECE_LAYER_NUM = 6;
const float Field::RADIUS = 45.0f;
std::vector<Vector2> Field::edges = std::vector<Vector2>();

Field::Field()
	:depthMagnitude(0.0f),
	 tiltDirection(Vector2()),
	 angleTilt(Vector3()),
	 localYvec(Vector3()),
	 isFallingBlock(false),
	 fallingLayerNum(0),
	 fallingBlockCount(0),
	 fallingBlockCountMax(0),
	 fallIntervalTimer(new Timer(INTERVAL_CREATE)),
	 lastTimeEdge(nullptr),
	 bonusPanelCount(0),
	 gottenCount(0),
	 previousGottenCount(0)
{
	if (edges.empty())
	{
		SetEdges();
	}

	Create(OBJLoader::GetModel("fieldEdge"));
	position = { 0,-5,0 };
	scale = { 50.0f, 5, 50.0f };
	color = { 0.1f , 0.1f, 0.1f, 0.3f };
	Object::Update();	

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::FIELD);

	pieces.clear();
	gottenPieces.clear();
	CreatePieces();
	infos.clear();
	setBonusPanelTimer = new Timer(setBonusPanelInterval);

	Initialize();
}

Field::~Field()
{	
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::FIELD);
	pieces.clear();
	delete fallIntervalTimer;
	blocks.clear();
	delete setBonusPanelTimer;
}

void Field::Initialize()
{
	ResetInfluences();
	CalcTilt();
	infos.clear();

	isFallingBlock = false;
	fallingLayerNum = 0;
	fallingBlockCount = 0;
	fallingBlockCountMax = 0;
	fallIntervalTimer->Reset();
	lastTimeEdge = nullptr;
	setBonusPanelTimer->Reset();
	bonusPanelCount = 0;
	SetBonusPanel();
	gottenCount = 0;
	previousGottenCount = 0;
}

void Field::Update()
{	
	static bool b = false;
#ifdef _DEBUG
	if (Input::TriggerKey(DIK_L))
	{
		if (!b)
		{
			StartFallingBlock();
			b = true;
		}
	}
#endif // _DEBUG

	if (isFallingBlock)
	{
		FallingBlock();
	}

	/*setBonusPanelTimer->Update();
	if (setBonusPanelTimer->IsTime())
	{
		SetBonusPanel();
	}*/

	CalcTilt();
	SetRotation(angleTilt);	

	Object::Update();	
}

void Field::DrawReady()
{
#ifdef _DEBUG
	ImGui::Begin("Field");		
	ImGui::SliderFloat("depthMag", &depthMagnitude, -MAX_DEPTH_MAGNITUDE, MAX_DEPTH_MAGNITUDE);
	ImGui::SliderFloat2("tiltDirection", &tiltDirection.x, -1.0f, 1.0f);
	ImGui::Text("angleTilt | x : %f | y : %f | z : %f", angleTilt.x, angleTilt.y, angleTilt.z);
	ImGui::Text("localYvec | x : %f | y : %f | z : %f", localYvec.x, localYvec.y, localYvec.z);
	ImGui::End();
#endif // _DEBUG
}

void Field::Draw()
{
	//Object::Draw();
}

void Field::CalcTilt()
{
	tiltDirection = Vector2();

	//Actor全体
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	for (auto a : actors)
	{
		if (a->GetActionState()->GetLabel() == ActionStateLabel::FALL ||
			a->GetActionState()->GetLabel() == ActionStateLabel::SPAWN)
		{
			continue;
		}

		Vector2 posVector = LocusUtility::Dim3ToDim2XZ(a->GetVirtualityPlanePosition());
		posVector = Vector2::Normalize(posVector) * a->GetWeight() * GetMultiplyingFactor(Vector3::Length(a->GetVirtualityPlanePosition()));
		tiltDirection += posVector;
	}

	for (int i = 0; i < influences.size(); i++)
	{
		Vector2 posVector = LocusUtility::Dim3ToDim2XZ(influences[i].pos);
		posVector = Vector2::Normalize(posVector) * influences[i].weight * GetMultiplyingFactor(Vector3::Length(influences[i].pos)) * -1;
		tiltDirection += posVector;
	}

	depthMagnitude = Vector2::Length(tiltDirection);
	if (depthMagnitude > MAX_DEPTH_MAGNITUDE)
	{
		depthMagnitude = MAX_DEPTH_MAGNITUDE;
	}
	tiltDirection = Vector2::Normalize(tiltDirection);

	Vector3 tmp = LocusUtility::Dim2XZToDim3(Vector2(tiltDirection.y, -tiltDirection.x));
	float degreeTilt = depthMagnitude * DEGREE_COEFFICIENT * PI / 180.0f;
	Quaternion quatTilt = quaternion(tmp, degreeTilt);
	angleTilt = LocusUtility::ToEuler(quatTilt);
	
	//
	XMMATRIX mat = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(angleTilt.x), XMConvertToRadians(angleTilt.y), XMConvertToRadians(angleTilt.z));
	localYvec = { mat.r[1].m128_f32[0], mat.r[1].m128_f32[1] , mat.r[1].m128_f32[2] };
}

float Field::GetMultiplyingFactor(const float arg_length)
{
	return arg_length / 300.0f;
}

void Field::DecideAllRidingPiece()
{
	//Player
	Player* player = ActorManager::GetInstance()->GetPlayer();
	DecideRidingPiece(player, player->GetVirtualityPlanePosition());

	//Enemy
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto e : enemies)
	{
		DecideRidingPiece(e, e->GetVirtualityPlanePosition());
	}
}

void Field::DecideAllCuttingStartPos()
{
	//Player
	Player* player = ActorManager::GetInstance()->GetPlayer();
	DecideCuttingStartPos(player, player->GetVirtualityPlanePosition(), player->GetDirection());

	//Enemy
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto e : enemies)
	{
		DecideCuttingStartPos(e, e->GetVirtualityPlanePosition(), e->GetDirection());
	}
}

void Field::DecideAllCuttingAngle()
{
	//Player
	Player* player = ActorManager::GetInstance()->GetPlayer();
	DecideCuttingAngle(player);

	//Enemy
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto e : enemies)
	{
		DecideCuttingAngle(e);
	}
}

void Field::DecideRidingPiece(Object* arg_obj, const Vector3& arg_pos)
{	
	Vector2 playerPos = LocusUtility::Dim3ToDim2XZ(arg_pos);
	float halfHeight = FieldPiece::GetFullOffset() * PIECE_LAYER_NUM;
	CuttingInfo* info = GetCuttingInfo(arg_obj);

	//列特定
	float adjustedZPos = -playerPos.y + halfHeight;
	if (adjustedZPos < 0)
	{
		adjustedZPos = 0;
	}
	else if (adjustedZPos > halfHeight * 2)
	{
		adjustedZPos = halfHeight * 2;
	}
	int columnNum = adjustedZPos / FieldPiece::GetFullOffset();
	if (columnNum == PIECE_LAYER_NUM * 2)
	{
		columnNum = PIECE_LAYER_NUM * 2 - 1;
	}

	//行特定
	for (int i = 0; i < pieces[columnNum].size(); i++)
	{
		std::vector<Vector2> piecePoints = pieces[columnNum][i]->GetPoints();
		for (int j = 0; j < 3; j++)
		{
			if (piecePoints[j].x == playerPos.x && piecePoints[j].y == playerPos.y)
			{
				info->ridingPiece = pieces[columnNum][i];
				return;
			}
		}

		float cross01 = Vector2::Cross(Vector2::Normalize(piecePoints[1] - piecePoints[0]), Vector2::Normalize(playerPos - piecePoints[0]));
		float cross12 = Vector2::Cross(Vector2::Normalize(piecePoints[2] - piecePoints[1]), Vector2::Normalize(playerPos - piecePoints[1]));
		float cross20 = Vector2::Cross(Vector2::Normalize(piecePoints[0] - piecePoints[2]), Vector2::Normalize(playerPos - piecePoints[2]));

		if (cross01 > 0 && cross12 > 0 && cross20 > 0)
		{
			info->ridingPiece = pieces[columnNum][i];
			return;
		}
		else if (cross01 < 0 && cross12 < 0 && cross20 < 0)
		{
			info->ridingPiece = pieces[columnNum][i];
			return;
		}
	}	

	info->ridingPiece = nullptr;
}

void Field::DecideCuttingStartPos(Object* arg_obj, const Vector3& arg_pos, const Vector3& arg_dir)
{		
	CuttingInfo* info = GetCuttingInfo(arg_obj);

	if (!info->ridingPiece)
	{
		info->cuttingStartPos = arg_pos;
		info->cuttingStartPosNum = 0;
		return;
	}

	float rateScore[3] = {0};
	std::vector<Vector2> piecePoints = info->ridingPiece->GetPoints();

	for (int i = 0; i < 3; i++)
	{
		//角度
		float eyeAngle = LocusUtility::Vector3XZToAngle(arg_dir);
		float posAngle = LocusUtility::Vector3XZToAngle(LocusUtility::Dim2XZToDim3(piecePoints[i], position.y) - arg_pos);
		float angleScore = fabsf(eyeAngle - posAngle);
		if (angleScore >= 180)
		{
			angleScore = 360 - angleScore;
		}
		rateScore[i] += Easing::Lerp(0.0f, 1.0f, angleScore / 180.0f);

		//距離
		float lengthScore = Vector2::Length(LocusUtility::Dim3ToDim2XZ(arg_pos) - piecePoints[i]);
		//rateScore[i] += Easing::Lerp(0.0f, 1.0f, lengthScore / FieldPiece::GetLowerTimeOffset()) * 40.0f;
	}

	int num = 0;
	for (int i = 1; i < 3; i++)
	{
		if (rateScore[num] > rateScore[i])
		{
			num = i;
		}
	}
	info->cuttingStartPos = LocusUtility::Dim2XZToDim3(piecePoints[num], position.y);
	info->cuttingStartPosNum = num;
}

void Field::DecideCuttingAngle(Object* arg_obj)
{	
	CuttingInfo* info = GetCuttingInfo(arg_obj);

	if (!info->ridingPiece)
	{
		info->cuttingAngle = 0;
		return;
	}
	
	//下向き
	if (info->ridingPiece->GetPieceDirection() == PieceDirection::Lower)
	{
		if (info->cuttingStartPosNum == 0)
		{
			info->cuttingAngle = 120;
		}
		else if (info->cuttingStartPosNum == 1)
		{
			info->cuttingAngle = 240;
		}
		else
		{
			info->cuttingAngle = 0;
		}
	}
	//上向き
	else
	{

		if (info->cuttingStartPosNum == 0)
		{
			info->cuttingAngle = 300;
		}
		else if (info->cuttingStartPosNum == 1)
		{
			info->cuttingAngle = 60;
		}
		else
		{
			info->cuttingAngle = 180;
		}
	}
}

void Field::FallingBlock()
{
	if (fallingLayerNum == 0)
	{
		if (fallingBlockCount == 0)
		{
			lastTimeEdge = new UnableThroughEdge(position, 50);
			ObjectManager::GetInstance()->Add(lastTimeEdge, false);
			fallIntervalTimer->SetLimit(INTERVAL_WAIT, true);
			fallingBlockCount++;
		}
		else if (lastTimeEdge->IsEndFallDown())
		{
			fallIntervalTimer->Update();

			if (fallIntervalTimer->IsTime())
			{
				fallingLayerNum++;

				innerRemainLayerNum = PIECE_LAYER_NUM - fallingLayerNum;
				topLayerPieceNum = (innerRemainLayerNum + 1) * 2 + 1;
				onesidePieceNum = innerRemainLayerNum * 4;
				fallingBlockCount = 0;
				fallingBlockCountMax = (topLayerPieceNum + onesidePieceNum) * 2;

				fallIntervalTimer->SetLimit(INTERVAL_CREATE, true);
				blocks.clear();
				lastTimeEdge = nullptr;
			}
		}
	}
	else
	{
		//ブロック落とすフェーズ
		if (fallIntervalTimer->GetLimit() == INTERVAL_CREATE)
		{
			//生成間隔待ち
			if (fallingBlockCount < fallingBlockCountMax)
			{
				fallIntervalTimer->Update();
				//1個落とし
				if (fallIntervalTimer->IsTime())
				{
					UnableThroughBlock* block = nullptr;
					int columnNum = 0;
					int rowNum = 0;

					//残っている最上段
					if (fallingBlockCount < topLayerPieceNum)
					{
						columnNum = fallingLayerNum - 1;
						rowNum = fallingBlockCount + (fallingLayerNum - 1) * 2;
					}
					//右側
					else if (fallingBlockCount < topLayerPieceNum + onesidePieceNum)
					{
						int trueCount = fallingBlockCount - topLayerPieceNum;
						columnNum = fallingLayerNum + (trueCount / 2);

						int layerSize = pieces[columnNum].size();

						if (trueCount % 2 == 0)
						{
							rowNum = layerSize - (fallingLayerNum * 2);
						}
						else
						{
							rowNum = layerSize - (fallingLayerNum * 2) + 1;
						}
					}
					//残っている最下段
					else if (fallingBlockCount < topLayerPieceNum + onesidePieceNum + topLayerPieceNum)
					{
						int trueCount = fallingBlockCount - topLayerPieceNum - onesidePieceNum;
						int startNum = topLayerPieceNum + (fallingLayerNum - 1) * 2 - 1;
						columnNum = pieces.size() - fallingLayerNum;
						rowNum = startNum - trueCount;
					}
					//左側
					else
					{
						int trueCount = fallingBlockCount - topLayerPieceNum - onesidePieceNum - topLayerPieceNum;
						columnNum = (PIECE_LAYER_NUM * 2 - fallingLayerNum - (trueCount / 2)) - 1;
						rowNum = 0;

						if (trueCount % 2 == 0)
						{
							rowNum = (fallingLayerNum - 1) * 2;
						}
						else
						{
							rowNum = (fallingLayerNum - 1) * 2 + 1;
						}
					}

					block = new UnableThroughBlock(pieces[columnNum][rowNum]->GetVirtualityPlanePosition(), 50, pieces[columnNum][rowNum]);
					ObjectManager::GetInstance()->Add(block, false);
					blocks.push_back(block);
					fallingBlockCount++;
					fallIntervalTimer->Reset();
				}
			}
			//全着地待ち
			else
			{
				for (auto b : blocks)
				{
					if (!b->IsEndFallDown())
					{
						return;
					}
				}
				fallIntervalTimer->SetLimit(INTERVAL_WAIT, true);
			}
		}
		//次の層落とし待ちフェーズ
		else
		{
			fallIntervalTimer->Update();
			//次の層へ
			if (fallIntervalTimer->IsTime())
			{
				fallingLayerNum++;

				innerRemainLayerNum = PIECE_LAYER_NUM - fallingLayerNum;
				topLayerPieceNum = (innerRemainLayerNum + 1) * 2 + 1;
				onesidePieceNum = innerRemainLayerNum * 4;
				fallingBlockCount = 0;
				fallingBlockCountMax = (topLayerPieceNum + onesidePieceNum) * 2;

				fallIntervalTimer->SetLimit(INTERVAL_CREATE, true);
				blocks.clear();

				if (fallingLayerNum > PIECE_LAYER_NUM)
				{
					isFallingBlock = false;
				}
			}
		}
	}
}

void Field::SetBonusPanel()
{
	const int randSetNum = std::rand() % 10 / 3 + 7;

	if (bonusPanelCount > 0)
	{
		for (auto vec : pieces)
		{
			for (auto p : vec)
			{
				p->ChangeIsBonus(false);
			}
		}
	}

	for (int i = 0; i < randSetNum; i++)
	{
		int yRand, xRand;
		do
		{
			yRand = std::rand() % (PIECE_LAYER_NUM * 2);
			xRand = std::rand() % pieces[yRand].size();
		} while (pieces[yRand][xRand]->IsBonus() || !pieces[yRand][xRand]->IsActive());
		pieces[yRand][xRand]->ChangeIsBonus(true);
	}

	setBonusPanelTimer->Reset();
	bonusPanelCount = randSetNum;
}

void Field::SetEdges()
{
	edges.clear();

	edges.push_back(Vector2( 1.000f,  0.000f));
	edges.push_back(Vector2( 0.500f, -0.866f));
	edges.push_back(Vector2(-0.500f, -0.866f));
	edges.push_back(Vector2(-1.000f,  0.000f));
	edges.push_back(Vector2(-0.500f,  0.866f));
	edges.push_back(Vector2( 0.500f,  0.866f));

	for (int i = 0; i < edges.size(); i++)
	{
		edges[i] = edges[i] * RADIUS;
	}
}

void Field::CreatePieces()
{
	const int TOP_PIECES_NUM = PIECE_LAYER_NUM * 2 + 1;
	const int CENTER_PIECES_NUM = PIECE_LAYER_NUM * 4 - 1;

	PieceDirection createDir = PieceDirection::Upper;
	int offsetCount = 0;
	Vector3 offset = Vector3();

	//上半分
	for (int i = 0; i < PIECE_LAYER_NUM; i++)
	{
		createDir = PieceDirection::Upper;
		offsetCount = -(TOP_PIECES_NUM + i * 2) / 2;

		std::vector<FieldPiece*> tmpVec;
		
		for (int j = 0; j < TOP_PIECES_NUM + i * 2; j++)
		{
			//オフセット計算
			offset.x = offsetCount * FieldPiece::GetSidewaysLength();
			offset.z = (PIECE_LAYER_NUM - i - 1) * FieldPiece::GetFullOffset();
			if (createDir == PieceDirection::Lower)
			{
				offset.z += FieldPiece::GetLowerTimeOffset();
			}
			else
			{
				offset.z += FieldPiece::GetUpperTimeOffset();
			}
			//offset.y = 1.0f;

			FieldPiece* piece = new FieldPiece(position + offset, createDir);
			piece->SetArrayIndex(Vector2(i, j));
			ObjectManager::GetInstance()->Add(piece);
			tmpVec.push_back(piece);

			//生成後
			if (createDir == PieceDirection::Lower)
			{
				createDir = PieceDirection::Upper;
			}
			else
			{
				createDir = PieceDirection::Lower;
			}
			offsetCount++;
		}
		pieces.push_back(tmpVec);
	}

	//下半分
	for (int i = 0; i < PIECE_LAYER_NUM; i++)
	{
		createDir = PieceDirection::Lower;
		offsetCount = -(CENTER_PIECES_NUM - i * 2) / 2;

		std::vector<FieldPiece*> tmpVec;

		for (int j = 0; j < CENTER_PIECES_NUM - i * 2; j++)
		{
			//オフセット計算
			offset.x = offsetCount * FieldPiece::GetSidewaysLength();
			offset.z = -i * FieldPiece::GetFullOffset();
			if (createDir == PieceDirection::Lower)
			{
				offset.z -= FieldPiece::GetUpperTimeOffset();
			}
			else
			{
				offset.z -= FieldPiece::GetLowerTimeOffset();
			}
			//offset.y = 1.0f;

			FieldPiece* piece = new FieldPiece(position + offset, createDir);
			piece->SetArrayIndex(Vector2(i + PIECE_LAYER_NUM, j));
			ObjectManager::GetInstance()->Add(piece);
			tmpVec.push_back(piece);

			//生成後
			if (createDir == PieceDirection::Lower)
			{
				createDir = PieceDirection::Upper;
			}
			else
			{
				createDir = PieceDirection::Lower;
			}
			offsetCount++;
		}
		pieces.push_back(tmpVec);
	}	
}

void Field::AddInfluence(const LocusFieldInfluence& arg_inf)
{
	influences.push_back(arg_inf);
}

void Field::ResetInfluences()
{
	influences.clear();

	depthMagnitude = 0.0f;
	tiltDirection = Vector2(0, 0);
	angleTilt = Vector3();
	localYvec = Vector3();

	for (auto vec : pieces)
	{
		for (auto p : vec)
		{
			p->ChangeIsActive(true);
			p->ChangeIsBlockade(false);
			p->ChangeIsBonus(false);
		}
	}
	gottenPieces.clear();
}

int Field::CutPanel(PanelCutLocus* arg_locus, int& arg_bonusCount, BaseGameActor* arg_target)
{
	auto vecPos = arg_locus->GetCuttedPanelPos();

	int returnVal = 0;	
	float halfHeight = FieldPiece::GetFullOffset() * PIECE_LAYER_NUM;
	int bonusCount = 0;

	for (int i = 0; i < vecPos.size(); i++)
	{
		//列特定
		float adjustedZPos = -vecPos[i].y + halfHeight;
		if (adjustedZPos < 0)
		{
			continue;
		}
		else if (adjustedZPos > halfHeight * 2)
		{
			continue;
		}
		int columnNum = adjustedZPos / FieldPiece::GetFullOffset();
		if (columnNum == PIECE_LAYER_NUM * 2)
		{
			columnNum = PIECE_LAYER_NUM * 2 - 1;
		}

		//行特定
		for (int j = 0; j < pieces[columnNum].size(); j++)
		{
			std::vector<Vector2> piecePoints = pieces[columnNum][j]->GetPoints();

			float cross01 = Vector2::Cross(Vector2::Normalize(piecePoints[1] - piecePoints[0]), Vector2::Normalize(vecPos[i] - piecePoints[0]));
			float cross12 = Vector2::Cross(Vector2::Normalize(piecePoints[2] - piecePoints[1]), Vector2::Normalize(vecPos[i] - piecePoints[1]));
			float cross20 = Vector2::Cross(Vector2::Normalize(piecePoints[0] - piecePoints[2]), Vector2::Normalize(vecPos[i] - piecePoints[2]));

			if (cross01 > 0 && cross12 > 0 && cross20 > 0)
			{
				if (pieces[columnNum][j]->IsActive() && pieces[columnNum][j]->IsCutable())
				{					
					if (pieces[columnNum][j]->IsBonus())
					{
						bonusCount++;
						bonusPanelCount--;
						pieces[columnNum][j]->ChangeIsBonus(false);
					}
					pieces[columnNum][j]->CutOneself(arg_target);
					AddInfluence(LocusFieldInfluence(pieces[columnNum][j]->GetVirtualityPlanePosition(), FieldPiece::GetWeight()));
					returnVal++;
					gottenPieces.push_back(pieces[columnNum][j]);
				}
				break;
			}
			else if (cross01 < 0 && cross12 < 0 && cross20 < 0)
			{
				if (pieces[columnNum][j]->IsActive() && pieces[columnNum][j]->IsCutable())
				{	
					if (pieces[columnNum][j]->IsBonus())
					{
						bonusCount++;
						bonusPanelCount--;
						pieces[columnNum][j]->ChangeIsBonus(false);
					}
					pieces[columnNum][j]->CutOneself(arg_target);
					AddInfluence(LocusFieldInfluence(pieces[columnNum][j]->GetVirtualityPlanePosition(), FieldPiece::GetWeight()));
					returnVal++;
					gottenPieces.push_back(pieces[columnNum][j]);
				}
				break;
			}
		}
	}

	//ボーナスまるのこ発生
	/*if (bonusCount == 1)
	{
		ItemEmitter* itemEmitter = ItemEmitter::GetInstance();
		itemEmitter->EmitEnergyItem(itemEmitter->GetEnergyItemEmitPosition(), RankEnergyItem::SILVER);
	}
	else if (bonusCount >= 2)
	{
		ItemEmitter* itemEmitter = ItemEmitter::GetInstance();
		itemEmitter->EmitEnergyItem(itemEmitter->GetEnergyItemEmitPosition(), RankEnergyItem::GOLD);
	}*/
	//ボーナス獲得数
	arg_bonusCount += bonusCount;

	if (bonusPanelCount <= 0)
	{
		SetBonusPanel();
	}

	previousGottenCount = gottenCount;
	gottenCount += returnVal;

	return returnVal;
}

void Field::ReviveGottenPanel(FieldPiece* arg_piece)
{
	for (auto itr = gottenPieces.begin(); itr != gottenPieces.end(); itr++)
	{
		if (*itr == arg_piece)
		{
			(*itr)->ChangeIsActive(true);
			gottenPieces.erase(itr);
			break;
		}
	}
}

void Field::StartFallingBlock()
{	
	isFallingBlock = true;
	fallingLayerNum = 0;
	innerRemainLayerNum = PIECE_LAYER_NUM - fallingLayerNum;
	topLayerPieceNum = (innerRemainLayerNum + 1) * 2 + 1;
	onesidePieceNum = innerRemainLayerNum * 4;
	fallingBlockCount = 0;
	fallingBlockCountMax = (topLayerPieceNum + onesidePieceNum) * 2;
	fallIntervalTimer->SetLimit(INTERVAL_CREATE, true);
	blocks.clear();
}

void Field::DecideCuttingInfo(Object* arg_object, const Vector3& arg_pos, const Vector3& arg_dir)
{
	DecideRidingPiece(arg_object, arg_pos);
	DecideCuttingStartPos(arg_object, arg_pos, arg_dir);
	DecideCuttingAngle(arg_object);
}

CuttingInfo* Field::GetCuttingInfo(Object* arg_pObject)
{
	auto itr = infos.find(arg_pObject);

	if (itr == infos.end())
	{		
		itr = infos.emplace(arg_pObject, CuttingInfo()).first;		
	}	

	return &itr->second;
}

FieldPiece* Field::IsRideGottenPanel(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius)
{
	for (auto p : gottenPieces)
	{
		if (p->IsRidden(arg_pos, arg_prePos, arg_radius))
		{
			return p;
		}		
	}
	return nullptr;
}

FieldPiece* Field::GetRespawnPiece(const ObjectRegistType arg_type)
{
	int colRnd = 0;
	int rowRnd = 0;

	do
	{
		colRnd = rand() % ((PIECE_LAYER_NUM - 1) * 2) + 1;
		if (colRnd == 1 || colRnd == PIECE_LAYER_NUM * 2 - 2)
		{
			rowRnd = rand() % (pieces[colRnd].size() - 4) + 2;
		}
		else
		{
			int rnd = rand() % 2;
			if (rnd == 0)
			{
				rowRnd = 2;
			}
			else
			{
				rowRnd = pieces[colRnd].size() - 3;
			}
		}
	} while (!pieces[colRnd][rowRnd]->IsActive());

	ChangeIsCutableWithAround(pieces[colRnd][rowRnd], false);
	return pieces[colRnd][rowRnd];
}

void Field::ChangeIsCutableWithAround(FieldPiece* arg_piece, const bool arg_flag)
{
	arg_piece->ChangeIsCutable(arg_flag);

	auto aroundPieces = GetAroundPiece(arg_piece, arg_piece->GetArrayIndex().x, arg_piece->GetArrayIndex().y);
	for (auto p : aroundPieces)
	{
		p->ChangeIsCutable(arg_flag);
	}
}

std::vector<FieldPiece*> Field::GetAroundPiece(FieldPiece* arg_piece, const int colNum, const int rowNum)
{
	return std::vector<FieldPiece*>();
}

FieldPiece* Field::GetRandomActivePiece()
{
	int columnNum, rowNum;
	
	do
	{
		columnNum = rand() % (PIECE_LAYER_NUM * 2);
		rowNum = rand() % pieces[columnNum].size();
	} while (!pieces[columnNum][rowNum]->IsActive());

	return pieces[columnNum][rowNum];
}

bool Field::IsNewFeverPlayer()
{
	const int feverNolma = 50;
	int nowRate = gottenCount / feverNolma;
	if (nowRate == 0)
	{
		return false;
	}
	int previousRate = previousGottenCount / feverNolma;
	return nowRate != previousRate;
}

float Field::GetLengthToFieldBorder(const int arg_index, const Vector3& arg_pos)
{
	Vector2 edgeStart = edges[arg_index];
	Vector2 edgeEnd = edges[(arg_index + 1) % 6];
	Vector2 actorPos = LocusUtility::Dim3ToDim2XZ(arg_pos);

	float a = edgeEnd.x - edgeStart.x;
	float b = edgeEnd.y - edgeStart.y;
	float a2 = a * a;
	float b2 = b * b;
	float r2 = a2 + b2;
	float tt = -(a * (edgeStart.x - actorPos.x) + b * (edgeStart.y - actorPos.y));
	
	if (tt < 0)
	{
		return Vector2::Length(edgeStart - actorPos);
	}
	else if (tt > r2)
	{
		return Vector2::Length(edgeEnd - actorPos);
	}
	
	float f1 = a * (edgeStart.y - actorPos.y) - b * (edgeStart.x - actorPos.x);
	return sqrtf((f1 * f1) / r2);
}

Vector3 Field::GetFieldBorderNormal(const int arg_index)
{
	if (arg_index >= edges.size())
	{
		return Vector3(1, 0, 0);
	}

	Vector3 edgeVec = LocusUtility::Dim2XZToDim3(Vector2(edges[(arg_index + 1) % 6] - edges[arg_index]));
	return Vector3::Normalize(Vector3(-edgeVec.z, 0, edgeVec.x));
}

float Field::GetRadius()
{
	return RADIUS;
}

std::vector<Vector2>& Field::GetEdges()
{
	return edges;
}

Vector3 Field::GetAngleTilt() const
{
	return angleTilt;
}

Vector3 Field::GetTilt()
{
	return LocusUtility::Dim2XZToDim3(tiltDirection);
}

float Field::GetDepthMagnitude() const
{
	return depthMagnitude;
}

std::vector<FieldPiece*>& Field::GetGottenPieces()
{
	return gottenPieces;
}

int Field::GetGottenCount()
{
	return gottenCount;
}
