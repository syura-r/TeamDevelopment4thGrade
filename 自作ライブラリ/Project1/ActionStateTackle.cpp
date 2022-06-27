#include "ActionStateTackle.h"

ActionStateTackle* ActionStateTackle::GetInstance()
{
	static ActionStateTackle* instance;
	return instance;
}

void ActionStateTackle::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
}

IActionState* ActionStateTackle::Update(BaseGameActor* arg_actor)
{
	return this;
}

void ActionStateTackle::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateTackle::GetLabel() const
{
	return ActionStateLabel::TACKLE;
}

ActionStateLabel ActionStateTackle::GetNext() const
{
	return next;
}
