#include "HelloAudio.h"

#include <Engine/Inc/Engine.h>

using namespace WOWGE;

HelloAudio::HelloAudio()
{

}

HelloAudio::~HelloAudio()
{

}

void HelloAudio::Initialize()
{
	DirectX::AUDIO_ENGINE_FLAGS audioEngineFlags{ DirectX::AudioEngine_Default };

	mAudioEngine = std::make_unique<DirectX::AudioEngine>(audioEngineFlags);

	mSoundEffect = std::make_unique<DirectX::SoundEffect>(mAudioEngine.get(), L"../../Assets/AudioSamples/record-scratch_zkkU_2VO.wav");

	mMusicEffect = std::make_unique<DirectX::SoundEffect>(mAudioEngine.get(), L"../../Assets/AudioSamples/live-the-moment_z1XWyUBO.wav");

	mSoundEffectInstance = mMusicEffect->CreateInstance();
}

int HelloAudio::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();
	if (!mAudioEngine->Update())
	{
		ASSERT(mAudioEngine->IsCriticalError(), "No Audio device is active");
	}

	if (inputSystem->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		mSoundEffect->Play();
	}

	mSoundEffectInstance->Play(true);

	if (inputSystem->IsKeyPressed(Input::KeyCode::ONE))
	{
		return 0;
	}
	return -1;
}
void HelloAudio::Terminate()
{
	mAudioEngine->Suspend();
}