#ifndef INCLUDED_SITTOTYPE_H
#define INCLUDED_SITTOTYPE_H

#include "OfficeWorker.h"

class SitToType :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer;
};

#endif // !INCLUDED_SITTOTYPE_H