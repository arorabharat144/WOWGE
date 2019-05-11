#ifndef INCLUDED_GRAPHICS_ANIMATIONBUILDER_H
#define INCLUDED_GRAPHICS_ANIMATIONBUILDER_H

#include "Animation.h"

namespace WOWGE
{
namespace Graphics
{

class AnimationBuilder
{
public:
	AnimationBuilder& AddPositionKey(Maths::Vector3 position);
	AnimationBuilder& AddRotationKey(Maths::Quaternion rotation);
	AnimationBuilder& AddScaleKey(Maths::Vector3 scale);

	AnimationBuilder& AddKey(Maths::Vector3 position, Maths::Quaternion rotation, Maths::Vector3 scale);

	AnimationBuilder& SetTime(float time);

	AnimationBuilder& Finalize();

	Animation GetAnimation();

private:
	Animation mAnimation;
	Keyframe<Maths::Vector3> mPositionKeyframe;
	Keyframe<Maths::Quaternion> mRotationKeyframe;
	Keyframe<Maths::Vector3> mScaleKeyframe;

	float mTime{ -1.0f };

};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_ANIMATIONBUILDER_H
