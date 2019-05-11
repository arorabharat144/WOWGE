#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

META_DERIVED_BEGIN(ColliderComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(mCenter, "Center")
	META_FIELD(mExtend, "Extend")
	META_FIELD(mColor, "Color")
	META_FIELD_END
META_CLASS_END;

ColliderComponent::ColliderComponent() :
	mTransformComponent{ nullptr },
	mCenter{},
	mExtend{ 1.0f, 1.0f, 1.0f }
{
	
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void ColliderComponent::Render()
{
	Maths::Vector3 center{ mCenter + mTransformComponent->GetPosition() };
	Maths::Vector3 min{ center - mExtend };
	Maths::Vector3 max{ center + mExtend };
	Maths::AABB aabb;
	aabb.SetMinMax(min, max);
	Graphics::SimpleDraw::AddAABB(aabb, mColor);
}