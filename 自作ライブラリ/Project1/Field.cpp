#include "Field.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "LocusDef.h"

Field::Field()
	:depthMagnitude(0.0f),
	 tiltDirection(Vector2()),
	 vecTilt(Vector3())
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
	depthMagnitude = 0.0f;
	tiltDirection = Vector2(0, 0);
	vecTilt = Vector3();
	CalcTilt();
}

void Field::Update()
{
	CalcTilt();
	SetRotation(vecTilt);

	Object::Update();
	collider->Update();
}

void Field::DrawReady()
{
#ifdef _DEBUG
	ImGui::Begin("Field");		
	ImGui::SliderFloat("depthMag", &depthMagnitude, 0, MAX_DEPTH_MAGNITUDE);
	ImGui::SliderFloat2("tiltDirection", &tiltDirection.x, -1.0f, 1.0f);
	ImGui::Text("vecTilt | x : %f | y : %f | z : %f", vecTilt.x, vecTilt.y, vecTilt.z);
	ImGui::End();
#endif // _DEBUG
}

void Field::CalcTilt()
{
	Vector3 tmp = LocusUtility::Dim2XZToDim3(Vector2(tiltDirection.y, -tiltDirection.x));
	float degreeTilt = depthMagnitude * DEGREE_COEFFICIENT * PI / 180.0f;
	Quaternion quatTilt = quaternion(tmp, degreeTilt);
	vecTilt = LocusUtility::ToEuler(quatTilt);
}
