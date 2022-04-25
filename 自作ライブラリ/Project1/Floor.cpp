#include "Floor.h"

#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "PlaneCollider.h"
#include "Texture.h"

Floor::Floor()
{
	Create(OBJLoader::GetModel("floor"));
	position = { 0,-5,0 };
	scale = { 5,1,5 };
	Object::Update();
	std::unique_ptr<BoxCollider> collider(new BoxCollider({ -25,-1,-15,0 },{20,1,23} ));
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->SetMove(true);
	collider->SetObject(this);
	collider->Update();
	CollisionManager::GetInstance()->AddCollider(collider.get());
	colliders.push_back(std::move(collider));
	std::unique_ptr<BoxCollider> collider2(new BoxCollider({ 15,-1,-15,0 }, { 20,1,23 }));
	collider2->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider2->SetMove(true);
	collider2->SetObject(this);
	collider2->Update();
	CollisionManager::GetInstance()->AddCollider(collider2.get());
	colliders.push_back(std::move(collider2));
	std::unique_ptr<BoxCollider> collider3(new BoxCollider({ -25,-1,25,0 }, { 20,1,23 }));
	collider3->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider3->SetMove(true);
	collider3->SetObject(this);
	collider3->Update();
	CollisionManager::GetInstance()->AddCollider(collider3.get());
	colliders.push_back(std::move(collider3));
	std::unique_ptr<BoxCollider> collider4(new BoxCollider({ 15,-1,25,0 }, { 20,1,23}));
	collider4->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider4->SetMove(true);
	collider4->SetObject(this);
	collider4->Update();
	CollisionManager::GetInstance()->AddCollider(collider4.get());
	colliders.push_back(std::move(collider4));
}


void Floor::Update()
{
	Object::Update();
	for (auto& it : colliders)
	{
		it->Update();
	}
}
