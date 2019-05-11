#include "Precompiled.h"
#include "AnimationClip.h"

#include "Bone.h"
#include "BoneAnimation.h"

#include <fstream>

using namespace WOWGE;
using namespace WOWGE::Graphics;

AnimationClip::AnimationClip() :
	duration{ 0.0f },
	ticksPerSecond{ 0.0f }
{

}

AnimationClip::~AnimationClip()
{
	for (uint32_t i{ 0 }; i < boneAnimations.size(); ++i)
	{
		SafeDelete(boneAnimations[i]);
	}
	boneAnimations.clear();
}

void AnimationClip::GetTransform(float time, Bone* bone, Maths::Matrix44& transform) const
{
	BoneAnimation* boneAnim{ boneAnimations[bone->index] };
	if (boneAnim)
	{
		transform = boneAnim->GetTransform(time);
	}
}

void AnimationClip::Serialize(std::ofstream& outputFile, uint32_t animationIndex)
{
	outputFile.write((char*)&duration, sizeof(uint32_t));
	outputFile.write((char*)&ticksPerSecond, sizeof(uint32_t));

	outputFile.write((char*)&totalNumberOfBones, sizeof(uint32_t));
	outputFile.write((char*)&numberOfBonesWithAnimations, sizeof(uint32_t));

	for (uint32_t i{ 0 }; i < totalNumberOfBones; ++i)
	{
		if (boneAnimations[i] != nullptr)
		{
			outputFile.write((char*)&boneAnimations[i]->boneIndex, sizeof(uint32_t));

			uint32_t numberOfPositionKeyFrames{ static_cast<uint32_t>(boneAnimations[i]->GetPositionKeyFrames().size()) };
			outputFile.write((char*)&numberOfPositionKeyFrames, sizeof(uint32_t));

			for (uint32_t j{ 0 }; j < numberOfPositionKeyFrames; ++j)
			{
				outputFile.write((char*)&boneAnimations[i]->GetPositionKeyFrames()[j].key, sizeof(Maths::Vector3));
				outputFile.write((char*)&boneAnimations[i]->GetPositionKeyFrames()[j].time, sizeof(float));
			}

			uint32_t numberOfRotationKeyFrames{ static_cast<uint32_t>(boneAnimations[i]->GetRotationKeyFrames().size()) };
			outputFile.write((char*)&numberOfRotationKeyFrames, sizeof(uint32_t));

			for (uint32_t j{ 0 }; j < numberOfRotationKeyFrames; ++j)
			{
				outputFile.write((char*)&boneAnimations[i]->GetRotationKeyFrames()[j].key, sizeof(Maths::Quaternion));
				outputFile.write((char*)&boneAnimations[i]->GetRotationKeyFrames()[j].time, sizeof(float));
			}

			uint32_t numberOfScaleKeyFrames{ static_cast<uint32_t>(boneAnimations[i]->GetScaleKeyFrames().size()) };
			outputFile.write((char*)&numberOfScaleKeyFrames, sizeof(uint32_t));

			for (uint32_t j{ 0 }; j < numberOfScaleKeyFrames; ++j)
			{
				outputFile.write((char*)&boneAnimations[i]->GetScaleKeyFrames()[j].key, sizeof(Maths::Vector3));
				outputFile.write((char*)&boneAnimations[i]->GetScaleKeyFrames()[j].time, sizeof(float));
			}
		}
	}
}

void AnimationClip::Reset()
{
	for (uint32_t i{ 0 }; i < boneAnimations.size(); ++i)
	{
		if (boneAnimations[i])
		{
			boneAnimations[i]->GetCurrentPositionKeyframeIndex() = 0;
			boneAnimations[i]->GetCurrentRotationKeyframeIndex() = 0;
			boneAnimations[i]->GetCurrentScaleKeyframeIndex() = 0;
		}
	}
}

void AnimationClip::LoadAnimation(const char* fileName)
{
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	Deserialize(inputFile);

	std::string animationName{ fileName };
	uint32_t found{ static_cast<uint32_t>(animationName.find_last_of(".")) };
	name = animationName.substr(0, found);
}

void AnimationClip::Deserialize(std::ifstream& inputFile)
{
	inputFile.read((char*)&duration, sizeof(uint32_t));
	inputFile.read((char*)&ticksPerSecond, sizeof(uint32_t));

	inputFile.read((char*)&totalNumberOfBones, sizeof(uint32_t));
	boneAnimations.resize(totalNumberOfBones, nullptr);

	inputFile.read((char*)&numberOfBonesWithAnimations, sizeof(uint32_t));

	for (uint32_t i{ 0 }; i < numberOfBonesWithAnimations; ++i)
	{
		BoneAnimation* boneAnimation{ new BoneAnimation() };
		inputFile.read((char*)&boneAnimation->boneIndex, sizeof(uint32_t));

		//boneAnimations[temp->boneIndex] = new BoneAnimation();

		uint32_t numberOfPositionKeyFrames;
		inputFile.read((char*)&numberOfPositionKeyFrames, sizeof(uint32_t));
		boneAnimation->GetPositionKeyFrames().resize(numberOfPositionKeyFrames);
		for (uint32_t j{ 0 }; j < numberOfPositionKeyFrames; ++j)
		{
			inputFile.read((char*)&boneAnimation->GetPositionKeyFrames()[j].key, sizeof(Maths::Vector3));
			inputFile.read((char*)&boneAnimation->GetPositionKeyFrames()[j].time, sizeof(float));

			boneAnimation->GetPositionKeyFrames()[j].time /= ticksPerSecond;
		}

		uint32_t numberOfRotationKeyFrames;
		inputFile.read((char*)&numberOfRotationKeyFrames, sizeof(uint32_t));
		boneAnimation->GetRotationKeyFrames().resize(numberOfRotationKeyFrames);
		for (uint32_t j{ 0 }; j < numberOfRotationKeyFrames; ++j)
		{
			inputFile.read((char*)&boneAnimation->GetRotationKeyFrames()[j].key, sizeof(Maths::Quaternion));
			inputFile.read((char*)&boneAnimation->GetRotationKeyFrames()[j].time, sizeof(float));

			boneAnimation->GetRotationKeyFrames()[j].time /= ticksPerSecond;
		}

		uint32_t numberOfScaleKeyFrames;
		inputFile.read((char*)&numberOfScaleKeyFrames, sizeof(uint32_t));
		boneAnimation->GetScaleKeyFrames().resize(numberOfScaleKeyFrames);
		for (uint32_t j{ 0 }; j < numberOfScaleKeyFrames; ++j)
		{
			inputFile.read((char*)&boneAnimation->GetScaleKeyFrames()[j].key, sizeof(Maths::Vector3));
			inputFile.read((char*)&boneAnimation->GetScaleKeyFrames()[j].time, sizeof(float));

			boneAnimation->GetScaleKeyFrames()[j].time /= ticksPerSecond;
		}


		boneAnimations[boneAnimation->boneIndex] = boneAnimation;
	}
}