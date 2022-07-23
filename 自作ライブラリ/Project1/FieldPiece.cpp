#include "FieldPiece.h"
#include "Field.h"
#include "LocusDef.h"
#include "OBJLoader.h"
#include "ActorManager.h"
#include "ParticleEmitter.h"
#include"Texture.h"
const float FieldPiece::SIZE = 7.5f / 0.866f / 2.0f;
const float FieldPiece::SIDEWAYS_LENGTH = 0.866f * SIZE;
const float FieldPiece::LOWER_TIME_OFFSET = 1.0f * SIZE;
const float FieldPiece::UPPER_TIME_OFFSET = 0.5f * SIZE;
const float FieldPiece::FULL_OFFSET = LOWER_TIME_OFFSET + UPPER_TIME_OFFSET;
const float FieldPiece::WEIGHT = 1.0f;
std::vector<Vector2> FieldPiece::basePoints = std::vector<Vector2>();

//float FieldPiece::baceColor[3] = { 18.0f / 255.0f, 34.0f / 255.0f ,83.0f / 255.0f };
//float FieldPiece::bonusColor[3] = { 188.0f / 255.0f, 143.0f / 255.0f ,36.0f / 255.0f };

FieldPiece::FieldPiece(const Vector3& arg_position, const PieceDirection arg_direction)
	:virtualityPlanePosition(arg_position),
	 dir(arg_direction),
	 arrayIndex(Vector2()),
	 isActive(true),
	 isBlockade(false),
	 cutterPos(Vector3()),
	 isBonus(false),
	 reviveTimer(new Timer(15 * 60)),
	 isCutable(true)
{
	SetPoints();
	position = virtualityPlanePosition;

	Create(OBJLoader::GetModel("fieldPiece"));

	Initialize();
}

FieldPiece::~FieldPiece()
{
}

void FieldPiece::Initialize()
{
	rotation = Vector3(0, 0, 0);
	if (dir == PieceDirection::Upper)
	{
		rotation.y = 180;
	}
	scale = Vector3(SIZE, 1, SIZE);
	isActive = true;
	isBlockade = false;
	isBonus = false;
	reviveTimer->Reset();
	isCutable = true;
	Object::Update();
}

void FieldPiece::Update()
{
	if (isBlockade)
	{
		return;
	}

	Revival();
	if (!isActive)
	{
		return;
	}

	if (dir == PieceDirection::Lower)
	{
		rotation = Vector3(0, 0, 0);
	}
	else
	{
		rotation = Vector3(0, 180, 0);
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
	Vector3 fieldRot = field->GetAngleTilt();
	XMMATRIX fieldRotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fieldRot.x), XMConvertToRadians(fieldRot.y), XMConvertToRadians(fieldRot.z));
	rotMat *= fieldRotMat;
	XMVECTOR rotQuat;
	XMMatrixDecompose(&XMVECTOR(), &rotQuat, &XMVECTOR(), rotMat);
	rotation = LocusUtility::ToEuler(quaternion(rotQuat.m128_f32[0], rotQuat.m128_f32[1], rotQuat.m128_f32[2], rotQuat.m128_f32[3]));

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());

	Object::Update();
}

void FieldPiece::Draw()
{
	if (!isActive || isBlockade)
	{
		return;
	}

	ChangeColorForRidden();

	Object::Draw();
}

bool FieldPiece::IsRidden(const Vector3& arg_pos, const Vector3& arg_prePos, const float arg_radius)
{
	for (int i = 0; i < points.size(); i++)
	{
		if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - arg_pos)) > SIZE + arg_radius)
		{
			continue;
		}

		Vector2 A = points[i];
		Vector2 B = points[(i + 1) % points.size()];
		Vector2 AO = LocusUtility::Dim3ToDim2XZ(arg_pos) - A;
		Vector2 BO = LocusUtility::Dim3ToDim2XZ(arg_pos) - B;
		Vector2 AB = B - A;
		Vector2 normalAB = Vector2::Normalize(AB);

		//¡“–‚½‚Á‚Ä‚¢‚é‚©
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > arg_radius)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{						
			return true;
		}

		if (Vector2::Length(AO) < arg_radius || Vector2::Length(BO) < arg_radius)
		{
			return true;
		}

		//’Ê‚è‰ß‚¬‚½‚©
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(arg_pos);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(arg_prePos);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			return true;
		}
	}

	return false;
}

void FieldPiece::CutOneself(Object* arg_obj)
{
	isActive = false;
	if (arg_obj)
	{
		cutterPos = arg_obj->GetPosition();
		ParticleEmitter::PieceGetEffect(position, scale, rotation, color, arg_obj);
	}
}

float FieldPiece::GetSize()
{
	return SIZE;
}

float FieldPiece::GetSidewaysLength()
{
	return SIDEWAYS_LENGTH;
}

float FieldPiece::GetLowerTimeOffset()
{
	return LOWER_TIME_OFFSET;
}

float FieldPiece::GetUpperTimeOffset()
{
	return UPPER_TIME_OFFSET;
}

float FieldPiece::GetFullOffset()
{
	return FULL_OFFSET;
}

Vector3 FieldPiece::GetVirtualityPlanePosition() const
{
	return virtualityPlanePosition;
}

float FieldPiece::GetWeight()
{
	return WEIGHT;
}

PieceDirection FieldPiece::GetPieceDirection() const
{
	return dir;
}

Vector2 FieldPiece::GetArrayIndex() const
{
	return arrayIndex;
}

void FieldPiece::SetArrayIndex(const Vector2& arg_index)
{
	arrayIndex = arg_index;
}

bool FieldPiece::IsActive()const
{
	return isActive;
}

void FieldPiece::ChangeIsActive(const bool arg_isActive)
{
	isActive = arg_isActive;
}

bool FieldPiece::IsBlockade()const
{
	return isBlockade;
}

void FieldPiece::ChangeIsBlockade(const bool arg_isBlockade)
{
	isBlockade = arg_isBlockade;
}

bool FieldPiece::IsBonus() const
{
	return isBonus;
}

void FieldPiece::ChangeIsBonus(const bool arg_isBonus)
{
	isBonus = arg_isBonus;
}

bool FieldPiece::IsCutable() const
{
	return isCutable;
}

void FieldPiece::ChangeIsCutable(const bool arg_flag)
{
	isCutable = arg_flag;
}

std::vector<Vector2>& FieldPiece::GetPoints()
{
	return points;
}

void FieldPiece::SetPoints()
{
	if (basePoints.empty())
	{
		basePoints.push_back(Vector2( 0.000f, -LOWER_TIME_OFFSET));
		basePoints.push_back(Vector2(-SIDEWAYS_LENGTH,  UPPER_TIME_OFFSET));
		basePoints.push_back(Vector2( SIDEWAYS_LENGTH,  UPPER_TIME_OFFSET));		
	}

	for (int i = 0; i < basePoints.size(); i++)
	{
		if (dir == PieceDirection::Lower)
		{
			points.push_back(basePoints[i]);
		}
		else
		{
			points.push_back(basePoints[i] * -1);
		}

		points[i] += LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
	}
}

void FieldPiece::ChangeColorForRidden()
{

	if (!isCutable)
	{
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		return;
	}

	if (isBonus)
	{
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		return;
	}

	ActorManager* aManager = ActorManager::GetInstance();
	Field* field = aManager->GetFields()[0];

	//Player
	CuttingInfo* info = aManager->GetFields()[0]->GetCuttingInfo((Object*)aManager->GetPlayer());
	if (info->ridingPiece == this)
	{
		//color = { 0.8f, 0.1f, 0.1f, 1.0f };
		return;
	}

	//Enemy
	auto enemies = aManager->GetStandardEnemies();
	for (auto e : enemies)
	{
		info = aManager->GetFields()[0]->GetCuttingInfo((Object*)e);
		if (info->ridingPiece == this)
		{
			//color = { 0.8f, 0.1f, 0.1f, 1.0f };
			return;
		}
	}
}

void FieldPiece::Revival()
{
	if (isActive)
	{
		return;
	}

	reviveTimer->Update();

	if (reviveTimer->IsTime())
	{
		isActive = true;
		reviveTimer->Reset();
		ActorManager::GetInstance()->GetFields()[0]->ReviveGottenPanel(this);
	}
}
