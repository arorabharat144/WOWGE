#ifndef INCLUDED_STANDTOSIT_H
#define INCLUDED_STANDTOSIT_H

#include "OfficeWorker.h"

class StandToSit :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_STANDTOSIT_H