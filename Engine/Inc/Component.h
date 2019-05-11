#ifndef INCLUDED_ENGINE_COMPONENT_H
#define INCLUDED_ENGINE_COMPONENT_H

namespace WOWGE
{
namespace Engine
{
class GameObject;

class Component
{
public:
	META_CLASS_DECLARE

	Component();
	virtual ~Component();

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}

	GameObject& GetOwner() { return *mGameObject; }
	const GameObject& GetOwner() const { return *mGameObject; }
private:
	friend class GameObject;

	GameObject* mGameObject;
};
} //namespace Engine
} //namespace WOWGE
#endif // !INCLUDED_ENGINE_COMPONENT_H
