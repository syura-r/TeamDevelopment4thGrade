#include "ActionStateWithstand.h"
#include "ActionStateMove.h"
#include "ActionStateFall.h"

ActionStateWithstand* ActionStateWithstand::GetInstance()
{
	static ActionStateWithstand* instance = new ActionStateWithstand();
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

	switch (next)
	{
	case ActionStateLabel::MOVE:
		return ActionStateMove::GetInstance();
		break;
	case ActionStateLabel::FALL:
		return ActionStateFall::GetInstance();
		break;
	default:
		break;
	}

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
