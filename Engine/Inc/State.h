#ifndef INCLUDED_ENGINE_STATE_H
#define INCLUDED_ENGINE_STATE_H

namespace WOWGE
{
namespace Engine
{

class State
{
public:
	virtual ~State() {}
	virtual void Initialize() = 0;
	virtual int Update(float deltaTime) = 0;
	virtual void Terminate() = 0;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_STATE_H
