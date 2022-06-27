#include "ActionStateMove.h"

ActionStateMove* ActionStateMove::GetInstance()
{
	static ActionStateMove* instance;
	return instance;
}

void ActionStateMove::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::CUT;
}

IActionState* ActionStateMove::Update(BaseGameActor* arg_actor)
{
	return this;
}

void ActionStateMove::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateMove::GetLabel() const
{
	return ActionStateLabel::MOVE;
}

ActionStateLabel ActionStateMove::GetNext() const
{
	return next;
}
