#include "Precompiled.h"
#include "Sound.h"

#include "AudioSystem.h"

using namespace WOWGE;
using namespace WOWGE::Audio;

Sound::Sound()
{

}

Sound::~Sound()
{

}

void Sound::LoadSound(const char* fileName)
{
	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, fileName, 1024);

	std::string soundName{ fileName };
	uint32_t found{ static_cast<uint32_t>(soundName.find_last_of(".")) };
	
	name = soundName.substr(0, found);
	effect = std::make_unique<DirectX::SoundEffect>(AudioSystem::Get()->mAudioEngine.get(), wbuffer);
	instance = effect->CreateInstance();
}

void Sound::Clear()
{
	AudioSystem::Get()->mAudioEngine->Suspend();

	instance->Stop();

	instance.reset();
	effect.reset();
}