#include "SoundManager.h"

SoundManager::SoundManager(std::string music)
{
	if (!BASS_Init(-1, 44100, 0, nullptr, nullptr))
		std::cout << '\n' << "Sound Error: Cant't initialise sound";

	sounds = new HSAMPLE[2]; //Best way to manipulate BASS is through use of arrays.

	sounds = new HSAMPLE[2]; //Best way to manipulate BASS is through the use of arrays.

	sounds[0] = loadSample(music.c_str());
	sounds[1] = loadSoundEffect(music.c_str());
	//cout << "Press P to pause music and R to resume music" << endl;
	Vol = 9;
	soundEffectChannel = BASS_SampleGetChannel(sounds[1], FALSE);
	BASS_ChannelSetAttribute(soundEffectChannel, BASS_ATTRIB_VOL, 0.8f);
}


SoundManager::~SoundManager() //Deleted music to free up memory
{
	BASS_Stop();
	BASS_Free();
}

HSAMPLE SoundManager::loadSample(const char * filename) const
{
	HSAMPLE music;
	music = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	return music;
}
HSAMPLE SoundManager::loadSoundEffect(const char * filename) const
{
	HSAMPLE SoundEffect;
	SoundEffect = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	return SoundEffect;
}
void SoundManager::playMusic() const
{

	HCHANNEL song = BASS_SampleGetChannel(sounds[0], FALSE);
	BASS_ChannelSetAttribute(soundEffectChannel, BASS_ATTRIB_VOL, 0.2f);
	//BASS_ChannelPlay(song, TRUE);

	if (!BASS_ChannelPlay(song, FALSE))
		std::cout << '\n' << "Can't play sample";
}
void SoundManager::playSoundEffect() const
{


	BASS_ChannelPlay(soundEffectChannel, FALSE);

	if (!BASS_ChannelPlay(soundEffectChannel, FALSE))
	{
		std::cout << '\n' << "Can't play sample";
	}
}
void SoundManager::StopSoundEffect() const
{
	BASS_ChannelPause(soundEffectChannel);
}
