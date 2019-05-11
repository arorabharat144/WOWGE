#include "Precompiled.h"
#include "AudioSystem.h"

using namespace WOWGE;
using namespace WOWGE::Audio;

namespace
{
	std::unique_ptr<AudioSystem> sAudioSystem{ nullptr };
}

void AudioSystem::StaticInitialize()
{
	ASSERT(sAudioSystem == nullptr, "[AudioSystem] System already initialized!");
	sAudioSystem = std::make_unique<AudioSystem>();
	sAudioSystem->Initialize();
}

AudioSystem* AudioSystem::Get()
{
	ASSERT(sAudioSystem != nullptr, "[AudioSystem] No system registered.");
	return sAudioSystem.get();
}

AudioSystem::AudioSystem():
	mAudioEngine{ nullptr }
{

}

AudioSystem::~AudioSystem()
{
	
}

void AudioSystem::Initialize()
{
	ASSERT(mAudioEngine == nullptr, "[AudioSystem] System already initialized.");

	DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_Default;
#if defined(_DEBUG)
	flags = flags | DirectX::AudioEngine_Debug;
#endif

	mAudioEngine = std::make_unique<DirectX::AudioEngine>(DirectX::AudioEngine{ flags });
}

void AudioSystem::Update()
{
	if (mAudioEngine && !mAudioEngine->Update())
	{
		// No audio device is active
		if (mAudioEngine->IsCriticalError())
		{
			LOG("[AudioSystem] Critical Error. Shutting down.");
		}
	}
}