#include "ActionStateMove.h"
#include "ActionStateTackle.h"
#include "ActionStateWithstand.h"
#include "ActionStateCut.h"
#include "ActionStateFall.h"
#include "Field.h"
#include "ActorManager.h"
#include "PanelCutLocus.h"

ActionStateMove* ActionStateMove::GetInstance()
{
	static ActionStateMove* instance = new ActionStateMove();
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

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(arg_actor);
	switch (next)
	{
	case ActionStateLabel::TACKLE:
		return ActionStateTackle::GetInstance();
		break;
	case ActionStateLabel::CUT:
		
		field->DecideCuttingInfo(arg_actor, arg_actor->virtualityPlanePosition, arg_actor->direction);
		arg_actor->panelCutLocus->SetCutPower(arg_actor->cutPower);
		info = field->GetCuttingInfo(arg_actor);
		arg_actor->panelCutLocus->Move(info->cuttingStartPos, info->cuttingAngle);
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
