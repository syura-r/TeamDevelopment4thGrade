#include "ActionStateTackle.h"
#include "ActionStateMove.h"
#include "ActionStateWithstand.h"
#include "ActionStateFall.h"

ActionStateTackle* ActionStateTackle::GetInstance()
{
	static ActionStateTackle* instance = new ActionStateTackle();
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

	FieldPiece* piece = nullptr;
	arg_actor->StayInTheField(next);
	if (next == ActionStateLabel::WITHSTAND)
	{
		return ActionStateWithstand::GetInstance();
	}
	else if (next == ActionStateLabel::FALL)
	{
		return ActionStateFall::GetInstance();
	}
	arg_actor->StayOnRemainPieces(next, piece);
	if (next == ActionStateLabel::WITHSTAND)
	{
		return ActionStateWithstand::GetInstance();
	}
	else if (next == ActionStateLabel::FALL)
	{
		return ActionStateFall::GetInstance();
	}

	if (next == ActionStateLabel::MOVE)
	{
		return ActionStateMove::GetInstance();
	}

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
