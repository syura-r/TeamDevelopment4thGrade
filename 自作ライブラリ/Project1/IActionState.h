#pragma once
#include "BaseGameActor.h"

enum class ActionStateLabel
{
	MOVE,
	TACKLE,
	BLOWN,
	WITHSTAND,
	CUT,
	FALL,
	SPAWN,
};

class IActionState
{
public:
	virtual void Initialize(BaseGameActor* arg_actor) = 0;
	virtual IActionState* Update(BaseGameActor* arg_actor) = 0;
	virtual void ShutDown(BaseGameActor* arg_actor) = 0;

	virtual ActionStateLabel GetLabel()const = 0;
	virtual ActionStateLabel GetNext()const = 0;

protected:
	friend class BaseGameActor;

	ActionStateLabel next;
};