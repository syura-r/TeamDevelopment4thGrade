#include "ActionStateFall.h"
#include "ActionStateSpawn.h"

ActionStateFall* ActionStateFall::GetInstance()
{
	static ActionStateFall* instance = new ActionStateFall();
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

	if (next == ActionStateLabel::SPAWN)
	{
		return ActionStateSpawn::GetInstance();
	}

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
