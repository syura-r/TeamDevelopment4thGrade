#pragma once
#include "BaseGameActor.h"

class IState
{
public:
	virtual IState* Update(BaseGameActor* arg_actor) = 0;	

protected:
	friend class BaseGameActor;
};