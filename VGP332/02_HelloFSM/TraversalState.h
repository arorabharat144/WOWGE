#ifndef INCLUDED_TRAVERSALSTATE_H
#define INCLUDED_TRAVERSALSTATE_H

#include "IronManAgent.h"

class TraversalState :public WOWGE::AI::State<IronManAgent>
{
public:

	void Enter(IronManAgent& agent) override;
	void Update(IronManAgent& agent, float deltaTime) override;
	void Exit(IronManAgent& agent) override;

private:
	WOWGE::Maths::Vector3 mStartPosition;
	WOWGE::Maths::Vector3 mDestination;

	WOWGE::Maths::Quaternion mStartRotation;
	WOWGE::Maths::Quaternion mEndRotation;

	float mTimer;
};

#endif // !INCLUDED_TRAVERSALSTATE_H
