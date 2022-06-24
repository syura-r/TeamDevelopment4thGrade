#pragma once

class IState
{
public:
	virtual IState* Update(BaseGameActor* arg_actor) = 0;

private:
	friend class BaseGameActor;		
};