#include "Field.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "LocusDef.h"
#include "ActorManager.h"
#include "Player.h"

Field::Field()
	:depthMagnitude(0.0f),
	 tiltDirection(Vector2()),
	 angleTilt(Vector3()),
	 localYvec(Vector3())
{
	Create(OBJLoader::GetModel("floor"));
	position = { 0,-5,0 };
	scale = { 5,1,5 };
	color = { 0,0.1f,0,1 };
	Object::Update();

	collider = new BoxCollider({ 0.0f, 0.0f, 0.0f, 0.0f }, Vector3(90.0f, 1.0f, 90.0f));
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetMove(true);
	collider->SetObject(this);
	collider->Update();
	CollisionManager::GetInstance()->AddCollider(collider);
}

Field::~Field()
{	
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
	XMMATRIX mat = XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationRollPitchYaw(angleTilt.x, angleTilt.y, angleTilt.z);
	localYvec = { mat.r[1].m128_f32[0], mat.r[1].m128_f32[1] , mat.r[1].m128_f32[2] };
}

float Field::GetMultiplyingFactor(const float arg_length)
{
	return arg_length / 300.0f;
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

Vector3 Field::GetAngleTilt() const
{
	return angleTilt;
}
