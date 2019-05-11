#ifndef INCLUDED_GRAPHICS_ANIMATIONCONTROLLER_H
#define INCLUDED_GRAPHICS_ANIMATIONCONTROLLER_H

#include "AnimationClip.h"

namespace WOWGE
{
namespace Graphics
{

class Model;

class AnimationController
{
public:
	AnimationController();
	~AnimationController();

	void Initialize(Model& model);

	void Play(const std::string& name, float deltaTime, bool isLooping = false);
	void Play(uint32_t clipIndex, float deltaTime, bool isLooping = false);
	void Update();

	void Reset(const std::string& name);
	void ResetAll();

	void Clear();

	void LoadAnimation(const char* fileName);

	AnimationClip* GetAnimationClip(const char* fileName);
private:
	std::vector<AnimationClip*> mAnimationClips;

	Model* mModel;

	float mTime{ 0.0f };
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_ANIMATIONCONTROLLER_H
