#ifndef INCLUDED_EVACUATE_H
#define INCLUDED_EVACUATE_H

#include "OfficeWorker.h"

class Evacuate :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
	WOWGE::Maths::Vector3 mDestination;
};

#endif // !INCLUDED_EVACUATE_H