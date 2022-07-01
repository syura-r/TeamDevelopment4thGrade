#include "ActionStateFall.h"

ActionStateFall* ActionStateFall::GetInstance()
{
	static ActionStateFall* instance;
	return instance;
}

void ActionStateFall::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
}

IActionState* ActionStateFall::Update(BaseGameActor* arg_actor)
{
	arg_actor->Fall();

	return this;
}

void ActionStateFall::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateFall::GetLabel() const
{
	return ActionStateLabel::FALL;
}

ActionStateLabel ActionStateFall::GetNext() const
{
	return next;
}
