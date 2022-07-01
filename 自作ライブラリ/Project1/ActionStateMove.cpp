#include "ActionStateMove.h"

ActionStateMove* ActionStateMove::GetInstance()
{
	static ActionStateMove* instance;
	return instance;
}

void ActionStateMove::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
	arg_actor->StartMove();
}

IActionState* ActionStateMove::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
	arg_actor->OnMove(next);
	return this;
}

void ActionStateMove::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndMove();
}

ActionStateLabel ActionStateMove::GetLabel() const
{
	return ActionStateLabel::MOVE;
}

ActionStateLabel ActionStateMove::GetNext() const
{
	return next;
}
