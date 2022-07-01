#include "ActionStateFall.h"

ActionStateFall* ActionStateFall::GetInstance()
{
	static ActionStateFall* instance;
	return instance;
}

void ActionStateFall::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::FALL;
	arg_actor->StartFall();
}

IActionState* ActionStateFall::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::FALL;
	arg_actor->OnFall(next);
	return this;
}

void ActionStateFall::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndFall();
}

ActionStateLabel ActionStateFall::GetLabel() const
{
	return ActionStateLabel::FALL;
}

ActionStateLabel ActionStateFall::GetNext() const
{
	return next;
}
