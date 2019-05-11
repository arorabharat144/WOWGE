#ifndef INCLUDED_TYPE_H
#define INCLUDED_TYPE_H

#include "OfficeWorker.h"

class Type :public WOWGE::AI::State<OfficeWorker>
{
public:

	void Enter(OfficeWorker& agent) override;
	void Update(OfficeWorker& agent, float deltaTime) override;
	void Exit(OfficeWorker& agent) override;

private:
	float mTimer{ 0.0f };
	float mWorkTimer{ 0.0f };
};

#endif // !INCLUDED_TYPE_H