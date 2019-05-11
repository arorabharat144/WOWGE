#ifndef INCLUDED_AUDIO_SOUND_H
#define INCLUDED_AUDIO_SOUND_H

namespace WOWGE
{
namespace Audio
{

class Sound
{
public:
	Sound();
	~Sound();

	std::string name;
	std::unique_ptr<DirectX::SoundEffect> effect;
	std::unique_ptr<DirectX::SoundEffectInstance> instance;

private:
	friend class SoundEffectController;

	void LoadSound(const char* fileName);
	void Clear();
};

} //namespace Audio
} //namespace WOWGE

#endif // !INCLUDED_AUDIO_SOUND_H
