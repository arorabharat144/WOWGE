#ifndef INCLUDED_AI_STEERINGBEHAVIOUR_H
#define INCLUDED_AI_STEERINGBEHAVIOUR_H

namespace WOWGE
{
namespace AI
{
class Agent;

class SteeringBehaviour
{
public:
	SteeringBehaviour() : mActive{ false } {}
	virtual ~SteeringBehaviour() {}

	virtual Maths::Vector3 Calculate(Agent& agent) = 0;
	virtual const char* GetName() const = 0;

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }
private:
	bool mActive;
};

} //namespace AI
} //namespace WOWGE

#endif // !INCLUDED_AI_STEERINGBEHAVIOUR_H
