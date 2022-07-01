#include "ActionStateCut.h"
#include "ActionStateWithstand.h"
#include "ActionStateFall.h"

ActionStateCut* ActionStateCut::GetInstance()
{
	static ActionStateCut* instance = new ActionStateCut();
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
