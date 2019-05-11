#ifndef INCLUDED_FILING_H
#define INCLUDED_FILING_H

#include "OfficeWorker.h"

class Filing :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer{ 0.0f };
};

#endif // !INCLUDED_FILING_H