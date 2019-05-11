#ifndef INCLUDED_FIGHTINGSTATE_H
#define INCLUDED_FIGHTINGSTATE_H

#include "IronManAgent.h"

class FightingState :public WOWGE::AI::State<IronManAgent>
{
public:

	void Enter(IronManAgent& agent) override;
	void Update(IronManAgent& agent, float deltaTime) override;
	void Exit(IronManAgent& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_FIGHTINGSTATE_H
