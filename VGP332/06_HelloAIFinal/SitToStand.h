#ifndef INCLUDED_SITTOSTAND_H
#define INCLUDED_SITTOSTAND_H

#include "OfficeWorker.h"

class SitToStand :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer{ 0.0f };
};

#endif // !INCLUDED_SITTOSTAND_H