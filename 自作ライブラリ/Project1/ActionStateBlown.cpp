#include "ActionStateBlown.h"

ActionStateBlown* ActionStateBlown::GetInstance()
{
	static ActionStateBlown* instance;
	return instance;
}

void ActionStateBlown::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::MOVE;
}

IActionState* ActionStateBlown::Update(BaseGameActor* arg_actor)
{
	return this;
}

void ActionStateBlown::ShutDown(BaseGameActor* arg_actor)
{
}

ActionStateLabel ActionStateBlown::GetLabel() const
{
	return ActionStateLabel::BLOWN;
}

ActionStateLabel ActionStateBlown::GetNext() const
{
	return next;
}
