#include "Precompiled.h"
#include "SoundEffectController.h"

#include "Sound.h"

using namespace WOWGE;
using namespace WOWGE::Audio;

SoundEffectController::SoundEffectController()
{

}

SoundEffectController::~SoundEffectController()
{
	
}

void SoundEffectController::Load(const char* fileName)
{
	std::unique_ptr<Sound> sound{ std::make_unique<Sound>() };

	sound->LoadSound(fileName);

	mSounds.emplace_back(std::move(sound));
}

void SoundEffectController::Clear()
{
	for (uint32_t i{ 0 }; i < mSounds.size(); ++i)
	{
		mSounds[i]->Clear();
		mSounds[i].reset();
	}

	mSounds.clear();
}

void SoundEffectController::Play(const std::string& name, bool isLooping)
{
	for (uint32_t i{ 0 }; i < mSounds.size(); ++i)
	{
		if (mSounds[i]->name.compare(name) == 0)
		{
			if (isLooping)
			{
				mSounds[i]->instance->Play(true);
			}
			else
			{
				mSounds[i]->effect->Play();
			}
		}
	}
}

void SoundEffectController::Play(uint32_t index, bool isLooping)
{
	if (isLooping)
	{
		mSounds[index]->instance->Play(true);
	}
	else
	{
		mSounds[index]->effect->Play();
	}
}

void SoundEffectController::Stop(const std::string& name)
{
	for (uint32_t i{ 0 }; i < mSounds.size(); ++i)
	{
		if (mSounds[i]->name.compare(name) == 0)
		{
			mSounds[i]->instance->Stop(true);
		}
	}
}

void SoundEffectController::Stop(uint32_t index)
{
	mSounds[index]->instance->Stop(true);
}