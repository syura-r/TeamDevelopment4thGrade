#pragma once
#include "IActionState.h"

class ActionStateTackle : public IActionState
{
public:
	static ActionStateTackle* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateTackle() {}
	~ActionStateTackle() {}
	ActionStateTackle(const ActionStateTackle& another) = delete;
	ActionStateTackle& operator=(const ActionStateTackle& another) = delete;
};