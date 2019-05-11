#ifndef INCLUDED_IDLE_H
#define INCLUDED_IDLE_H

#include "OfficeWorker.h"

class Idle :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_IDLE_H