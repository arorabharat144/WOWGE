#ifndef INCLUDED_MININGSTATE_H
#define INCLUDED_MININGSTATE_H

#include "IronManAgent.h"

class MiningState :public WOWGE::AI::State<IronManAgent>
{
public:

	void Enter(IronManAgent& agent) override;
	void Update(IronManAgent& agent, float deltaTime) override;
	void Exit(IronManAgent& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_MININGSTATE_H
