#ifndef INCLUDED_AI_STATE_H
#define INCLUDED_AI_STATE_H

namespace WOWGE
{
namespace AI
{

template<class AgentType>
class State
{
public:
	virtual ~State() {}
	virtual void Enter(AgentType& agentType) = 0;
	virtual void Update(AgentType& agentType, float deltaTime) = 0;
	virtual void Exit(AgentType& agentType) = 0;


};

} //namespace AI
} //namespace WOWGE

#endif // !INCLUDED_AI_STATE_H
