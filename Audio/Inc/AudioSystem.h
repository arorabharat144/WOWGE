#ifndef INCLUDED_AUDIO_AUDIOSYSTEM_H
#define INCLUDED_AUDIO_AUDIOSYSTEM_H

namespace DirectX
{
	class AudioEngine;
}

namespace WOWGE
{
namespace Audio
{
class AudioSystem
{
public:
	static void StaticInitialize();
	static AudioSystem* Get();

public:
	AudioSystem();
	~AudioSystem();

	AudioSystem(const AudioSystem&) = delete;
	AudioSystem& operator=(const AudioSystem&) = delete;

	void Initialize();

	void Update();

private:
	friend class Sound;

	std::unique_ptr<DirectX::AudioEngine> mAudioEngine;
};

} //namespace Audio
} //namespace WOWGE

#endif // !INCLUDED_AUDIO_AUDIOSYSTEM_H
