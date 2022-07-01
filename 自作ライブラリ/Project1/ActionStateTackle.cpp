#include "ActionStateTackle.h"

ActionStateTackle* ActionStateTackle::GetInstance()
{
	static ActionStateTackle* instance;
	return instance;
}

void ActionStateTackle::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::TACKLE;
	arg_actor->StartTackle();
}

IActionState* ActionStateTackle::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::TACKLE;
	arg_actor->OnTackle(next);
	return this;
}

void ActionStateTackle::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndTackle();
}

ActionStateLabel ActionStateTackle::GetLabel() const
{
	return ActionStateLabel::TACKLE;
}

ActionStateLabel ActionStateTackle::GetNext() const
{
	return next;
}
