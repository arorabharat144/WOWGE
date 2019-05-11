#ifndef INCLUDED_GRAPHICS_ANIMATION_H
#define INCLUDED_GRAPHICS_ANIMATION_H

#include "Keyframe.h"

namespace WOWGE
{
namespace Graphics
{

class Animation
{
public:
	//Maths::Matrix44 Animate(float deltaTime, bool isLooping);

	Maths::Matrix44 GetTransform(float time);

	Keyframe<Maths::Vector3>& GetPositionKeyframe(float time);
	Keyframe<Maths::Quaternion>& GetRotationKeyframe(float time);
	Keyframe<Maths::Vector3>& GetScaleKeyframe(float time);

	std::vector<Keyframe<Maths::Vector3>>& GetPositionKeyFrames();
	std::vector<Keyframe<Maths::Quaternion>>& GetRotationKeyFrames();
	std::vector<Keyframe<Maths::Vector3>>& GetScaleKeyFrames();

	void AddPositionKeyFrame(const Keyframe<Maths::Vector3>& keyframe);
	void AddRotationKeyFrame(const Keyframe<Maths::Quaternion>& keyframe);
	void AddScaleKeyFrame(const Keyframe<Maths::Vector3>& keyframe);

	uint32_t& GetCurrentPositionKeyframeIndex();
	uint32_t& GetCurrentRotationKeyframeIndex();
	uint32_t& GetCurrentScaleKeyframeIndex();

private:

	std::vector<Keyframe<Maths::Vector3>> mPositionKeyframes;
	std::vector<Keyframe<Maths::Quaternion>> mRotationKeyframes;
	std::vector<Keyframe<Maths::Vector3>> mScaleKeyframes;

	uint32_t mCurrentPositionKeyframeIndex{ 0 };
	uint32_t mCurrentRotationKeyframeIndex{ 0 };
	uint32_t mCurrentScaleKeyframeIndex{ 0 };
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_ANIMATION_H
