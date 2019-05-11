#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

AnimationBuilder& AnimationBuilder::AddPositionKey(Maths::Vector3 position)
{
	mPositionKeyframe.key = position;

	mPositionKeyframe.time = mTime;

	mAnimation.AddPositionKeyFrame(mPositionKeyframe);

	return *this;

}

AnimationBuilder& AnimationBuilder::AddRotationKey(Maths::Quaternion rotation)
{
	mRotationKeyframe.key = rotation;

	mRotationKeyframe.time = mTime;

	mAnimation.AddRotationKeyFrame(mRotationKeyframe);

	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(Maths::Vector3 scale)
{
	mScaleKeyframe.key = scale;

	mScaleKeyframe.time = mTime;

	mAnimation.AddScaleKeyFrame(mScaleKeyframe);

	return *this;
}

AnimationBuilder& AnimationBuilder::AddKey(Maths::Vector3 position, Maths::Quaternion rotation, Maths::Vector3 scale)
{
	mPositionKeyframe.key = position;
	mRotationKeyframe.key = rotation;
	mScaleKeyframe.key = scale;

	mPositionKeyframe.time = mTime;
	mRotationKeyframe.time = mTime;
	mScaleKeyframe.time = mTime;

	mAnimation.AddPositionKeyFrame(mPositionKeyframe);
	mAnimation.AddRotationKeyFrame(mRotationKeyframe);
	mAnimation.AddScaleKeyFrame(mScaleKeyframe);

	return *this;
}

AnimationBuilder& AnimationBuilder::SetTime(float time)
{
	mTime = time;
	return *this;
}

AnimationBuilder& AnimationBuilder::Finalize()
{
	mAnimation.AddPositionKeyFrame(mPositionKeyframe);
	mAnimation.AddRotationKeyFrame(mRotationKeyframe);
	mAnimation.AddScaleKeyFrame(mScaleKeyframe);

	return *this;
}

Animation AnimationBuilder::GetAnimation()
{
	return mAnimation;
}