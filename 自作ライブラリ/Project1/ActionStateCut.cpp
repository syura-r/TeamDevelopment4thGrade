#include "ActionStateCut.h"

ActionStateCut* ActionStateCut::GetInstance()
{
	static ActionStateCut* instance;
	return instance;
}

void ActionStateCut::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
}

IActionState* ActionStateCut::Update(BaseGameActor* arg_actor)
{
	return this;
}

void ActionStateCut::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateCut::GetLabel() const
{
	return ActionStateLabel::CUT;
}

ActionStateLabel ActionStateCut::GetNext() const
{
	return next;
}
