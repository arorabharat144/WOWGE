#ifndef INCLUDED_SLEEPINGSTATE_H
#define INCLUDED_SLEEPINGSTATE_H

#include "IronManAgent.h"

class SleepingState :public WOWGE::AI::State<IronManAgent>
{
public:

	void Enter(IronManAgent& agent) override;
	void Update(IronManAgent& agent, float deltaTime) override;
	void Exit(IronManAgent& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_SLEEPINGSTATE_H
