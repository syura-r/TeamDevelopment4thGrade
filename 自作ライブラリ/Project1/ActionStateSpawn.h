#pragma once
#include "IActionState.h"

class ActionStateSpawn : public IActionState
{
public:
	static ActionStateSpawn* GetInstance();

	virtual void Initialize(BaseGameActor* arg_actor) override;
	virtual IActionState* Update(BaseGameActor* arg_actor) override;
	virtual void ShutDown(BaseGameActor* arg_actor) override;
	virtual ActionStateLabel GetLabel() const override;
	virtual ActionStateLabel GetNext() const override;

private:
	friend class BaseGameActor;

	ActionStateSpawn() {}
	~ActionStateSpawn() {}
	ActionStateSpawn(const ActionStateSpawn& another) = delete;
	ActionStateSpawn& operator=(const ActionStateSpawn& another) = delete;
};