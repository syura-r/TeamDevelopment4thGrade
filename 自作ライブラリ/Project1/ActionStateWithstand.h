#pragma once
#include "IActionState.h"

class ActionStateWithstand : public IActionState
{
public:
	static ActionStateWithstand* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateWithstand() {}
	~ActionStateWithstand() {}
	ActionStateWithstand(const ActionStateWithstand& another) = delete;
	ActionStateWithstand& operator=(const ActionStateWithstand& another) = delete;
};