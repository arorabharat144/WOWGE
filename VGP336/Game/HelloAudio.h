#ifndef INCLUDED_GAME_HELLOAUDIO_H
#define INCLUDED_GAME_HELLOAUDIO_H

#include <Engine/Inc/Engine.h>
#include <External/DirectXTK/Inc/Audio.h>

class HelloAudio :public WOWGE::Engine::State
{
public:
	HelloAudio();
	~HelloAudio() override;

	void Initialize() override;
	int Update(float deltaTime) override;
	void Terminate() override;
private:
	std::unique_ptr<DirectX::AudioEngine> mAudioEngine;

	std::unique_ptr<DirectX::SoundEffect> mSoundEffect;
	std::unique_ptr<DirectX::SoundEffect> mMusicEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> mSoundEffectInstance;
};

#endif // !INCLUDED_GAME_HELLOAUDIO_H
