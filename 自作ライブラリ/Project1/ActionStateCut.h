#pragma once
#include "IActionState.h"

class ActionStateCut : public IActionState
{
	static ActionStateCut* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateCut() {}
	~ActionStateCut() {}
	ActionStateCut(const ActionStateCut& another) = delete;
	ActionStateCut& operator=(const ActionStateCut& another) = delete;
};