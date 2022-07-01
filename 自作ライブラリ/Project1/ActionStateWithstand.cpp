#include "ActionStateWithstand.h"

ActionStateWithstand* ActionStateWithstand::GetInstance()
{
	static ActionStateWithstand* instance;
	return instance;
}

void ActionStateWithstand::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::WITHSTAND;
	arg_actor->StartWithstand();
}

IActionState* ActionStateWithstand::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::WITHSTAND;
	arg_actor->OnWithstand(next);
	return this;
}

void ActionStateWithstand::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndWithstand();
}

ActionStateLabel ActionStateWithstand::GetLabel() const
{
	return ActionStateLabel::WITHSTAND;
}

ActionStateLabel ActionStateWithstand::GetNext() const
{
	return next;
}
