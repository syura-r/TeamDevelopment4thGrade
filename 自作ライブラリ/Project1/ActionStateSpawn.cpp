#include "ActionStateSpawn.h"
#include "ActionStateMove.h"

ActionStateSpawn* ActionStateSpawn::GetInstance()
{
	static ActionStateSpawn* instance = new ActionStateSpawn();
	return instance;
}

void ActionStateSpawn::Initialize(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::SPAWN;
	arg_actor->StartSpawn();
}

IActionState* ActionStateSpawn::Update(BaseGameActor* arg_actor)
{
	next = ActionStateLabel::SPAWN;
	arg_actor->OnSpawn(next);

	if (next == ActionStateLabel::MOVE)
	{
		return ActionStateMove::GetInstance();
	}

	return this;
}

void ActionStateSpawn::ShutDown(BaseGameActor* arg_actor)
{
	arg_actor->EndSpawn();
}

ActionStateLabel ActionStateSpawn::GetLabel() const
{
	return ActionStateLabel::SPAWN;
}

ActionStateLabel ActionStateSpawn::GetNext() const
{
	return next;
}
