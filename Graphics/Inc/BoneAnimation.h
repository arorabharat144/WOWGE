#ifndef INCLUDED_GRAPHICS_BONEANIMATION_H
#define INCLUDED_GRAPHICS_BONEANIMATION_H

#include "Animation.h"

namespace WOWGE
{

namespace Graphics
{

class BoneAnimation :public Animation
{
public:
	BoneAnimation() {}
	~BoneAnimation() {}

	uint32_t boneIndex{ 0 };
};

using BoneAnimations = std::vector<BoneAnimation*>;

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_BONEANIMATION_H
