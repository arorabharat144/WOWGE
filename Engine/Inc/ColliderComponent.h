#ifndef INCLUDED_ENGINE_COLLIDERCOMPONENT_H
#define INCLUDED_ENGINE_COLLIDERCOMPONENT_H

#include "Component.h"
#include <Math/Inc/Maths.h>

namespace WOWGE
{
namespace Engine
{
class TransformComponent;

class ColliderComponent : public Component
{
public:
	META_CLASS_DECLARE

	ColliderComponent();
	~ColliderComponent() override;

	void Initialize() override;
	void Render() override;

private:
	const TransformComponent* mTransformComponent;

	Maths::Vector3 mCenter;
	Maths::Vector3 mExtend;
	Maths::Vector4 mColor;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_COLLIDERCOMPONENT_H