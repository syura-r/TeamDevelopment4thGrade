#pragma once
#include "IActionState.h"

class ActionStateMove : public IActionState
{
public:
	static ActionStateMove* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateMove() {}
	~ActionStateMove() {}
	ActionStateMove(const ActionStateMove& another) = delete;
	ActionStateMove& operator=(const ActionStateMove& another) = delete;
};