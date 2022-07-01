#include "ActionStateMove.h"
#include "ActionStateTackle.h"
#include "ActionStateWithstand.h"
#include "ActionStateCut.h"
#include "ActionStateFall.h"

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
	if (arg_actor->IsChangeMoveToTackle())
	{
		next = ActionStateLabel::TACKLE;
	}
	else if (arg_actor->IsChangeMoveToCut())
	{
		next = ActionStateLabel::CUT;
	}
	else
	{
		next = ActionStateLabel::MOVE;
		arg_actor->OnMove(next);
	}
	
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

	switch (next)
	{
	case ActionStateLabel::TACKLE:
		return ActionStateTackle::GetInstance();
		break;
	case ActionStateLabel::CUT:
		return ActionStateCut::GetInstance();
		break;
	default:
		break;
	}
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
