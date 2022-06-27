#pragma once
#include "IActionState.h"

class ActionStateFall : public IActionState
{
public:
	static ActionStateFall* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateFall() {}
	~ActionStateFall() {}
	ActionStateFall(const ActionStateFall& another) = delete;
	ActionStateFall& operator=(const ActionStateFall& another) = delete;
};