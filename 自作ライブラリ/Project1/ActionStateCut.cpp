#include "ActionStateCut.h"

ActionStateCut* ActionStateCut::GetInstance()
{
	static ActionStateCut* instance;
	return instance;
}

void ActionStateCut::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::CUT;
	arg_actor->StartCut();
}

IActionState* ActionStateCut::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::CUT;
	arg_actor->OnCut(next);
	return this;
}

void ActionStateCut::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndCut();
}

ActionStateLabel ActionStateCut::GetLabel() const
{
	return ActionStateLabel::CUT;
}

ActionStateLabel ActionStateCut::GetNext() const
{
	return next;
}
