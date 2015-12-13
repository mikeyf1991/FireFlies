#pragma once

#include "Fireflies\bass.h"
#include <iostream>
#include <string>
//------------------------------Sound Manager Class---------------------------------
class SoundManager
{
private:
	HSAMPLE *sounds;
	int Vol;
public:

	explicit SoundManager(std::string music);
	~SoundManager(); //Deletes music from application to free memory space
	HSAMPLE loadSample(const char * filename) const;
	HSAMPLE loadSoundEffect(const char * filename) const;
	void playMusic() const;
	void playSoundEffect() const;
	void StopSoundEffect() const;
	//Need to update user input class.
	HCHANNEL soundEffectChannel;
	int getVolume() const
	{ return Vol; }
	void setVolume(int v){ Vol = v; }
};
//--------------------------------------------------------------------------------