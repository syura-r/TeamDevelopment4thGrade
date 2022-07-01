#include "ActionStateBlown.h"

ActionStateBlown* ActionStateBlown::GetInstance()
{
	static ActionStateBlown* instance;
	return instance;
}

void ActionStateBlown::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::BLOWN;
	arg_actor->StartBlown();
}

IActionState* ActionStateBlown::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::BLOWN;
	arg_actor->OnBlown(next);
	return this;
}

void ActionStateBlown::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndBlown();
}

ActionStateLabel ActionStateBlown::GetLabel() const
{
	return ActionStateLabel::BLOWN;
}

ActionStateLabel ActionStateBlown::GetNext() const
{
	return next;
}
