#ifndef INCLUDED_AI_WALLAVOIDANCEBEHAVIOUR_H
#define INCLUDED_AI_WALLAVOIDANCEBEHAVIOUR_H

#include "SteeringBehaviour.h"

namespace WOWGE
{
namespace AI
{

class WallAvoidanceBehaviour : public SteeringBehaviour
{
public:
	WallAvoidanceBehaviour();

	Maths::Vector3 Calculate(Agent& agent) override;
	const char* GetName() const override { return "WallAvoidance"; }

	void CreateFeelers(Agent& agent);

	Maths::Vector3 mFeelers[3];
	Maths::Vector3 point;
private:
	float mFeelerLength{ 100.0f };


};

} //namespace AI
} //namespace WOWGE

#endif // #include INCLUDED_AI_WALLAVOIDANCEBEHAVIOUR_H