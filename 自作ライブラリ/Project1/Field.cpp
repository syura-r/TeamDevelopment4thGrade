#include "Field.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "LocusDef.h"
#include "ActorManager.h"
#include "Player.h"
#include "StandardEnemy.h"
#include "FieldPiece.h"
#include "Input.h"
#include "Easing.h"
#include "UnableThroughBlock.h"

const int Field::PIECE_LAYER_NUM = 6;
std::vector<Vector2> Field::edges = std::vector<Vector2>();

Field::Field()
	:depthMagnitude(0.0f),
	 tiltDirection(Vector2()),
	 angleTilt(Vector3()),
	 localYvec(Vector3()),
	 info(CuttingInfo())
{
	if (edges.empty())
	{
		SetEdges();
	}

	Create(OBJLoader::GetModel("fieldEdge"));
	position = { 0,-5,0 };
	scale = { RADIUS,1,RADIUS };
	color = { 0.1f ,0.1f, 0.1f,1 };
	Object::Update();

	collider = new BoxCollider({ 0.0f, 0.0f, 0.0f, 0.0f }, Vector3(90.0f, 1.0f, 90.0f));
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetMove(true);
	collider->SetObject(this);
	collider->Update();
	CollisionManager::GetInstance()->AddCollider(collider);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("Field", this);

	pieces.clear();
	gottenPieces.clear();
	CreatePieces();
}

Field::~Field()
{	
	ActorManager::GetInstance()->DeleteObject(this);	
	pieces.clear();
}

void Field::Initialize()
{
	ResetInfluences();
	CalcTilt();
	info.ridingPiece = nullptr;
}

void Field::Update()
{	
	CalcTilt();
	SetRotation(angleTilt);	
	DecidePlayerRidingPiece();
	DecidePlayerCuttingStartPos();
	DecidePlayerCuttingAngle();

	if (Input::TriggerKey(DIK_L))
	{
		UnableThroughBlock* block = new UnableThroughBlock(pieces[0][0]->GetVirtualityPlanePosition(), 50, pieces[0][0]);
		ObjectManager::GetInstance()->Add(block);
	}

	Object::Update();
	collider->Update();
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
	Object::Draw();
}

void Field::CalcTilt()
{
	tiltDirection = Vector2();

	Player* player = ActorManager::GetInstance()->GetPlayer();
	if (player)
	{
		Vector2 posVector = LocusUtility::Dim3ToDim2XZ(player->GetVirtualityPlanePosition());
		posVector = Vector2::Normalize(posVector) * player->GetWeight() * GetMultiplyingFactor(Vector3::Length(player->GetVirtualityPlanePosition()));
		tiltDirection += posVector;
	}

	std::vector<StandardEnemy*> enemies = ActorManager::GetInstance()->GetStandardEnemies();
	for (auto itr = enemies.begin(); itr != enemies.end(); itr++)
	{
		Vector2 posVector = LocusUtility::Dim3ToDim2XZ((*itr)->GetVirtualityPlanePosition());
		posVector = Vector2::Normalize(posVector) * (*itr)->GetWeight() * GetMultiplyingFactor(Vector3::Length((*itr)->GetVirtualityPlanePosition()));
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

void Field::DecidePlayerRidingPiece()
{
	Player* player = ActorManager::GetInstance()->GetPlayer();
	Vector2 playerPos = LocusUtility::Dim3ToDim2XZ(player->GetVirtualityPlanePosition());
	float halfHeight = FieldPiece::GetFullOffset() * PIECE_LAYER_NUM;

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
				info.ridingPiece = pieces[columnNum][i];
				return;
			}
		}

		float cross01 = Vector2::Cross(Vector2::Normalize(piecePoints[1] - piecePoints[0]), Vector2::Normalize(playerPos - piecePoints[0]));
		float cross12 = Vector2::Cross(Vector2::Normalize(piecePoints[2] - piecePoints[1]), Vector2::Normalize(playerPos - piecePoints[1]));
		float cross20 = Vector2::Cross(Vector2::Normalize(piecePoints[0] - piecePoints[2]), Vector2::Normalize(playerPos - piecePoints[2]));

		if (cross01 > 0 && cross12 > 0 && cross20 > 0)
		{
			info.ridingPiece = pieces[columnNum][i];
			return;
		}
		else if (cross01 < 0 && cross12 < 0 && cross20 < 0)
		{
			info.ridingPiece = pieces[columnNum][i];
			return;
		}
	}	

	info.ridingPiece = nullptr;
}

void Field::DecidePlayerCuttingStartPos()
{
	Player* player = ActorManager::GetInstance()->GetPlayer();
	Vector3 playerPos = player->GetVirtualityPlanePosition();

	if (!info.ridingPiece)
	{
		info.cuttingStartPos = playerPos;
		info.cuttingStartPosNum = 0;
		return;
	}

	float rateScore[3] = {0};
	std::vector<Vector2> piecePoints = info.ridingPiece->GetPoints();

	for (int i = 0; i < 3; i++)
	{
		//角度
		float eyeAngle = LocusUtility::Vector3XZToAngle(player->GetDirection());
		float posAngle = LocusUtility::Vector3XZToAngle(LocusUtility::Dim2XZToDim3(piecePoints[i], position.y) - player->GetVirtualityPlanePosition());
		float angleScore = fabsf(eyeAngle - posAngle);
		if (angleScore >= 180)
		{
			angleScore = 360 - angleScore;
		}
		rateScore[i] += Easing::Lerp(0.0f, 1.0f, angleScore / 180.0f);

		//距離
		float lengthScore = Vector2::Length(LocusUtility::Dim3ToDim2XZ(player->GetVirtualityPlanePosition()) - piecePoints[i]);
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
	info.cuttingStartPos = LocusUtility::Dim2XZToDim3(piecePoints[num], position.y);
	info.cuttingStartPosNum = num;
}

void Field::DecidePlayerCuttingAngle()
{
	if (!info.ridingPiece)
	{
		info.cuttingAngle = 0;
		return;
	}
	
	//下向き
	if (info.ridingPiece->GetPieceDirection() == PieceDirection::Lower)
	{
		if (info.cuttingStartPosNum == 0)
		{
			info.cuttingAngle = 120;
		}
		else if (info.cuttingStartPosNum == 1)
		{
			info.cuttingAngle = 240;
		}
		else
		{
			info.cuttingAngle = 0;
		}
	}
	//上向き
	else
	{

		if (info.cuttingStartPosNum == 0)
		{
			info.cuttingAngle = 300;
		}
		else if (info.cuttingStartPosNum == 1)
		{
			info.cuttingAngle = 60;
		}
		else
		{
			info.cuttingAngle = 180;
		}
	}
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
		}
	}
	gottenPieces.clear();
}

int Field::CutPanel(std::vector<Vector2>& arg_vecPos)
{
	int returnVal = 0;
	Player* player = ActorManager::GetInstance()->GetPlayer();
	float halfHeight = FieldPiece::GetFullOffset() * PIECE_LAYER_NUM;

	for (int i = 0; i < arg_vecPos.size(); i++)
	{
		//列特定
		float adjustedZPos = -arg_vecPos[i].y + halfHeight;
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

			float cross01 = Vector2::Cross(Vector2::Normalize(piecePoints[1] - piecePoints[0]), Vector2::Normalize(arg_vecPos[i] - piecePoints[0]));
			float cross12 = Vector2::Cross(Vector2::Normalize(piecePoints[2] - piecePoints[1]), Vector2::Normalize(arg_vecPos[i] - piecePoints[1]));
			float cross20 = Vector2::Cross(Vector2::Normalize(piecePoints[0] - piecePoints[2]), Vector2::Normalize(arg_vecPos[i] - piecePoints[2]));

			if (cross01 > 0 && cross12 > 0 && cross20 > 0)
			{
				if (pieces[columnNum][j]->IsActive())
				{
					pieces[columnNum][j]->ChangeIsActive(false);
					AddInfluence(LocusFieldInfluence(pieces[columnNum][j]->GetVirtualityPlanePosition(), FieldPiece::GetWeight()));
					returnVal++;
					gottenPieces.push_back(pieces[columnNum][j]);
				}
				break;
			}
			else if (cross01 < 0 && cross12 < 0 && cross20 < 0)
			{
				if (pieces[columnNum][j]->IsActive())
				{
					pieces[columnNum][j]->ChangeIsActive(false);
					AddInfluence(LocusFieldInfluence(pieces[columnNum][j]->GetVirtualityPlanePosition(), FieldPiece::GetWeight()));
					returnVal++;
					gottenPieces.push_back(pieces[columnNum][j]);
				}
				break;
			}
		}
	}

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

FieldPiece* Field::GetPlayerRidingPiece()
{
	return info.ridingPiece;
}

Vector3 Field::GetPlayerCuttingStartPos()
{
	return info.cuttingStartPos;
}

float Field::GetPlayerCuttingAngle()
{
	return info.cuttingAngle;
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
