#ifndef INCLUDED_GRAPHICS_ANIMATIONCLIP_H
#define INCLUDED_GRAPHICS_ANIMATIONCLIP_H

#include "BoneAnimation.h"

namespace WOWGE
{
namespace Graphics
{

struct Bone;

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void GetTransform(float time, Bone* bone, Maths::Matrix44& transform) const;

	void Serialize(std::ofstream& outputFile, uint32_t animationIndex);

	BoneAnimations boneAnimations;
	std::string name;
	float duration;
	float ticksPerSecond;
	uint32_t totalNumberOfBones;
	uint32_t numberOfBonesWithAnimations;

private:
	friend class AnimationController;

	void Reset();

	void LoadAnimation(const char* fileName);

	void Deserialize(std::ifstream& inputFile);
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_ANIMATIONCLIP_H
