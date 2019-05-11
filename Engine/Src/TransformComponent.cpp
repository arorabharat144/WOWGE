#include "Precompiled.h"
#include "TransformComponent.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(mPosition, "Position")
	META_FIELD(mHeading, "Heading")
	META_FIELD(mUp, "Up")
	META_FIELD(mScale, "Scale")
	META_FIELD_END
META_CLASS_END;

TransformComponent::TransformComponent() :
	mPosition{},
	mHeading{ 0.0f, 0.0f, 1.0f },
	mUp{ 0.0f, 1.0f, 0.0f },
	mScale{ 1.0f, 1.0f, 1.0f }
{
	
}

TransformComponent::~TransformComponent()
{

}