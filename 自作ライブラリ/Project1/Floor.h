#pragma once
#include "BoxCollider.h"
#include "TouchAbleObject.h"

class Floor :
    public Object
{
public:
	Floor();
	void Update() override;
private:
	std::vector<std::unique_ptr<BoxCollider>> colliders;
};

