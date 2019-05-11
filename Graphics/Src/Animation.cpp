#include "Precompiled.h"
#include "Animation.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

Maths::Matrix44 Animation::GetTransform(float time)
{
	ASSERT(mPositionKeyframes.size() > 0 || mRotationKeyframes.size() > 0 || mScaleKeyframes.size() > 0, "[Animation] Animation keyframes are empty");

	if (mPositionKeyframes.size() <= 1 && mRotationKeyframes.size() <= 1 && mScaleKeyframes.size() <= 1)
	{
		return
		{
			Maths::Matrix44::Scaling(mScaleKeyframes[0].key.x, mScaleKeyframes[0].key.y, mScaleKeyframes[0].key.z)
			*
			Maths::QuaternionToMatrix(mRotationKeyframes[0].key)
			*
			Maths::Matrix44::Translation(mPositionKeyframes[0].key.x, mPositionKeyframes[0].key.y, mPositionKeyframes[0].key.z)
		};
	}

	Maths::Matrix44 translationMatrix;
	for (uint32_t i = mCurrentPositionKeyframeIndex; i < mPositionKeyframes.size(); ++i)
	{
		if (i == mPositionKeyframes.size() - 1)
		{
			mCurrentPositionKeyframeIndex = i;

			translationMatrix = Maths::Matrix44::Translation(mPositionKeyframes[i].key.x, mPositionKeyframes[i].key.y, mPositionKeyframes[i].key.z);
			break;
		}
		else if (time >= mPositionKeyframes[i].time && time < mPositionKeyframes[i + 1].time)
		{
			mCurrentPositionKeyframeIndex = i;

			float localTime{ time };
			if (mPositionKeyframes[i].time != 0.0f)
			{
				localTime = fmod(localTime, mPositionKeyframes[i].time);
			}
			Maths::Vector3 position = Maths::Lerp(mPositionKeyframes[i].key, mPositionKeyframes[i + 1].key, Maths::Clamp(localTime / (mPositionKeyframes[i + 1].time - mPositionKeyframes[i].time), 0.0f, 1.0f));

			translationMatrix = Maths::Matrix44::Translation(position.x, position.y, position.z);
			break;
		}
	}

	Maths::Matrix44 rotationMatrix;
	for (uint32_t i = mCurrentRotationKeyframeIndex; i < mRotationKeyframes.size(); ++i)
	{
		if (i == mRotationKeyframes.size() - 1)
		{
			mCurrentRotationKeyframeIndex = i;

			rotationMatrix = Maths::QuaternionToMatrix(mRotationKeyframes[i].key);
			break;
		}
		else if (time >= mRotationKeyframes[i].time && time < mRotationKeyframes[i + 1].time)
		{
			mCurrentRotationKeyframeIndex = i;

			float localTime{ time };
			if (mRotationKeyframes[i].time != 0.0f)
			{
				localTime = fmod(localTime, mRotationKeyframes[i].time);
			}
			Maths::Quaternion rotation = Maths::Slerp(mRotationKeyframes[i].key, mRotationKeyframes[i + 1].key, Maths::Clamp(localTime / (mRotationKeyframes[i + 1].time - mRotationKeyframes[i].time), 0.0f, 1.0f));

			rotationMatrix = Maths::QuaternionToMatrix(rotation);
			break;
		}
	}

	Maths::Matrix44 scaleMatrix;
	for (uint32_t i = mCurrentScaleKeyframeIndex; i < mScaleKeyframes.size(); ++i)
	{
		if (i == mScaleKeyframes.size() - 1)
		{
			mCurrentScaleKeyframeIndex = i;

			scaleMatrix = Maths::Matrix44::Scaling(mScaleKeyframes[i].key.x, mScaleKeyframes[i].key.y, mScaleKeyframes[i].key.z);
			break;
		}
		else if (time >= mScaleKeyframes[i].time && time < mScaleKeyframes[i + 1].time)
		{
			mCurrentScaleKeyframeIndex = i;

			float localTime{ time };
			if (mScaleKeyframes[i].time != 0.0f)
			{
				localTime = fmod(localTime, mScaleKeyframes[i].time);
			}
			Maths::Vector3 scale = Maths::Lerp(mScaleKeyframes[i].key, mScaleKeyframes[i + 1].key, Maths::Clamp(localTime / (mScaleKeyframes[i + 1].time - mScaleKeyframes[i].time), 0.0f, 1.0f));

			scaleMatrix = Maths::Matrix44::Scaling(scale.x, scale.y, scale.z);
			break;
		}
	}

	return 
	{
		scaleMatrix * rotationMatrix * translationMatrix
	};
}

Keyframe<Maths::Vector3>& Animation::GetPositionKeyframe(float time)
{
	uint32_t i = 0;
	for (; i < mPositionKeyframes.size(); ++i)
	{
		if (mPositionKeyframes[i].time == time)
		{
			break;
		}
	}
	return mPositionKeyframes[i];
}

Keyframe<Maths::Quaternion>& Animation::GetRotationKeyframe(float time)
{
	uint32_t i = 0;
	for (; i < mRotationKeyframes.size(); ++i)
	{
		if (mRotationKeyframes[i].time == time)
		{
			break;
		}
	}
	return mRotationKeyframes[i];
}

Keyframe<Maths::Vector3>& Animation::GetScaleKeyframe(float time)
{
	uint32_t i = 0;
	for (; i < mScaleKeyframes.size(); ++i)
	{
		if (mScaleKeyframes[i].time == time)
		{
			break;
		}
	}
	return mScaleKeyframes[i];
}

std::vector<Keyframe<Maths::Vector3>>& Animation::GetPositionKeyFrames()
{
	return mPositionKeyframes;
}
std::vector<Keyframe<Maths::Quaternion>>& Animation::GetRotationKeyFrames()
{
	return mRotationKeyframes;
}
std::vector<Keyframe<Maths::Vector3>>& Animation::GetScaleKeyFrames()
{
	return mScaleKeyframes;
}

void Animation::AddPositionKeyFrame(const Keyframe<Maths::Vector3>& keyframe)
{
	mPositionKeyframes.push_back(keyframe);
}

void Animation::AddRotationKeyFrame(const Keyframe<Maths::Quaternion>& keyframe)
{
	mRotationKeyframes.push_back(keyframe);
}

void Animation::AddScaleKeyFrame(const Keyframe<Maths::Vector3>& keyframe)
{
	mScaleKeyframes.push_back(keyframe);
}

uint32_t& Animation::GetCurrentPositionKeyframeIndex()
{
	return mCurrentPositionKeyframeIndex;
}

uint32_t& Animation::GetCurrentRotationKeyframeIndex()
{
	return mCurrentRotationKeyframeIndex;
}

uint32_t& Animation::GetCurrentScaleKeyframeIndex()
{
	return mCurrentScaleKeyframeIndex;
}