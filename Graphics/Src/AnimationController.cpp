#include "Precompiled.h"
#include "AnimationController.h"

#include "Model.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

AnimationController::AnimationController()

{

}
AnimationController::~AnimationController()
{
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		SafeDelete(mAnimationClips[i]);
	}
	mAnimationClips.clear();
}

void AnimationController::Initialize(Model& model)
{
	mModel = &model;
}

void AnimationController::Play(const std::string& name, float deltaTime, bool isLooping)
{
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		if (mAnimationClips[i]->name.compare(name) == 0)
		{
			mTime += deltaTime;
			if (isLooping)
			{
				if (mTime >= (mAnimationClips[i]->duration / mAnimationClips[i]->ticksPerSecond))
				{
					mTime = 0.0f;
					mAnimationClips[i]->Reset();
				}
			}
			for (uint32_t j{ 0 }; j < mModel->GetNumberOfBones(); ++j)
			{
				mAnimationClips[i]->GetTransform(mTime, mModel->GetBones()[j], mModel->GetBones()[j]->transform);
			}
			break;
		}
	}
}

void AnimationController::Play(uint32_t clipIndex, float deltaTime, bool isLooping)
{
	mTime += deltaTime;
	if (isLooping)
	{
		if (mTime >= (mAnimationClips[clipIndex]->duration / mAnimationClips[clipIndex]->ticksPerSecond))
		{
			mTime = 0.0f;
			mAnimationClips[clipIndex]->Reset();
		}
	}
	for (uint32_t j{ 0 }; j < mModel->GetNumberOfBones(); ++j)
	{
		mAnimationClips[clipIndex]->GetTransform(mTime, mModel->GetBones()[j], mModel->GetBones()[j]->transform);
	}
}

void AnimationController::Update()
{
	for (uint32_t i{ 0 }; i < mModel->GetNumberOfBones(); ++i)
	{
		Graphics::Bone* bone{ mModel->GetBones()[i] };
		mModel->GetBoneWorldTransforms()[i] = bone->transform;
		while (bone->parent)
		{
			mModel->GetBoneWorldTransforms()[i] = mModel->GetBoneWorldTransforms()[i] * bone->parent->transform;

			bone = bone->parent;
		}
	}
}

void AnimationController::Reset(const std::string& name)
{
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		if (mAnimationClips[i]->name.compare(name) == 0)
		{
			mTime = 0.0f;
			mAnimationClips[i]->Reset();
			break;
		}
	}
}

void AnimationController::ResetAll()
{
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		mTime = 0.0f;
		mAnimationClips[i]->Reset();
	}
}

void AnimationController::Clear()
{
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		SafeDelete(mAnimationClips[i]);
	}
	mAnimationClips.clear();
}

void AnimationController::LoadAnimation(const char* fileName)
{
	AnimationClip* animationClip{ new AnimationClip() };
	animationClip->LoadAnimation(fileName);

	mAnimationClips.emplace_back(animationClip);
}

AnimationClip* AnimationController::GetAnimationClip(const char* fileName)
{
	AnimationClip* currentAnimationClip{ nullptr };
	for (uint32_t i{ 0 }; i < mAnimationClips.size(); ++i)
	{
		if (mAnimationClips[i]->name.compare(fileName) == 0)
		{
			currentAnimationClip = mAnimationClips[i];
			break;
		}
	}

	return currentAnimationClip;
}