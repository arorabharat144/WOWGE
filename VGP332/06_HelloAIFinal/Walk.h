#ifndef INCLUDED_WALK_H
#define INCLUDED_WALK_H

#include "OfficeWorker.h"

class Walk :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
	WOWGE::Maths::Vector3 mDestination;
};

#endif // !INCLUDED_WALK_H