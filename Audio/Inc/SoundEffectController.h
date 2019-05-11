#ifndef INCLUDED_AUDIO_SOUNDEFFECTCONTROLLER_H
#define INCLUDED_AUDIO_SOUNDEFFECTCONTROLLER_H

namespace DirectX
{
	class SoundEffect;
	class SoundEffectInstance;
}

namespace WOWGE
{
namespace Audio
{

class Sound;

class SoundEffectController
{

public:
	SoundEffectController();
	~SoundEffectController();

	SoundEffectController(const SoundEffectController&) = delete;
	SoundEffectController& operator=(const SoundEffectController&) = delete;

	void Load(const char* fileName);
	void Clear();

	void Play(const std::string& name, bool isLooping = false);
	void Play(uint32_t index, bool isLooping = false);

	void Stop(const std::string& name);
	void Stop(uint32_t index);
private:

	std::vector<std::unique_ptr<Sound>> mSounds;
};

} //namespace Audio
} //namespace WOWGE

#endif // !INCLUDED_AUDIO_SOUNDEFFECTCONTROLLER_H
