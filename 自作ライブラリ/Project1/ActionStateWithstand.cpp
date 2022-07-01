#include "ActionStateWithstand.h"

ActionStateWithstand* ActionStateWithstand::GetInstance()
{
	static ActionStateWithstand* instance;
	return instance;
}

void ActionStateWithstand::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
}

IActionState* ActionStateWithstand::Update(BaseGameActor* arg_actor)
{
	return this;
}

void ActionStateWithstand::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateWithstand::GetLabel() const
{
	return ActionStateLabel::WITHSTAND;
}

ActionStateLabel ActionStateWithstand::GetNext() const
{
	return next;
}
