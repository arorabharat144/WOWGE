#ifndef INCLUDED_TALK_H
#define INCLUDED_TALK_H

#include "OfficeWorker.h"

class Talk :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_TALK_H