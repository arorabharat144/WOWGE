#ifndef INCLUDED_HUDDLE_H
#define INCLUDED_HUDDLE_H

#include "OfficeWorker.h"

class Huddle :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_HUDDLE_H