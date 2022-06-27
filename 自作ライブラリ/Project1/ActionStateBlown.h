#pragma once
#include "IActionState.h"

class ActionStateBlown : public IActionState
{
public:
	static ActionStateBlown* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	ActionStateBlown() {}
	~ActionStateBlown() {}
	ActionStateBlown(const ActionStateBlown& another) = delete;
	ActionStateBlown& operator=(const ActionStateBlown& another) = delete;
};