#include "ActionStateBlown.h"
#include "ActionStateMove.h"
#include "ActionStateWithstand.h"
#include "ActionStateFall.h"

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
