#include "Field.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "LocusDef.h"
#include "ActorManager.h"
#include "Player.h"
#include "StandardEnemy.h"

const Vector2 Field::LOWER_LIMIT = Vector2(-45, -45);
const Vector2 Field::UPPER_LIMIT = Vector2(45, 45);
std::vector<Vector2> Field::edges = std::vector<Vector2>();

Field::Field()
	:depthMagnitude(0.0f),
	 tiltDirection(Vector2()),
	 angleTilt(Vector3()),
	 localYvec(Vector3())
{
	if (edges.empty())
	{
		SetEdges();
	}

	Create(OBJLoader::GetModel("Hexagon"));
	position = { 0,-5,0 };
	scale = { 45,1,45 };
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
}

Field::~Field()
{	
	ActorManager::GetInstance()->DeleteObject(this);
}

void Field::Initialize()
{
	ResetInfluences();
	CalcTilt();
}

void Field::Update()
{
	CalcTilt();
	SetRotation(angleTilt);

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
