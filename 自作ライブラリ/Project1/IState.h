#pragma once
#include "BaseGameActor.h"

enum class StateLabel
{
	Move,
	Tackle,
	Blown,
	Withstand,
	Cut,
	Fall
};

class IState
{
public:
	virtual IState* Update(BaseGameActor* arg_actor) = 0;	
	virtual StateLabel GetLabel()const = 0;

protected:
	friend class BaseGameActor;
};