#ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_ENGINE_TRANSFORMCOMPONENT_H

#include "Component.h"
#include <Math/Inc/Maths.h>

namespace WOWGE
{
namespace Engine
{

class TransformComponent : public Component
{
public:
	META_CLASS_DECLARE

	TransformComponent();
	~TransformComponent() override;

	void SetPosition(const Maths::Vector3& position) { mPosition = position; }

	const Maths::Vector3& GetPosition() const { return mPosition; }
	const Maths::Vector3& GetHeading() const { return mHeading; }
	const Maths::Vector3& GetUp() const { return mUp; }
	const Maths::Vector3& GetScale() const { return mScale; }

private:
	Maths::Vector3 mPosition;
	Maths::Vector3 mHeading;
	Maths::Vector3 mUp;
	Maths::Vector3 mScale;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_TRANSFORMCOMPONENT_H