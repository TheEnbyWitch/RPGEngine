#pragma once
#include "rSoundInstance.h"
#include "rStreamInstance.h"
#define SNDINIT if(!this->Initialized) return;
class rSound
{
public:
	rSound();
	~rSound();

	bool Initialized = false;

	bool Init();
	
	void PlayMusic();

	void PlaySample(char * sndName);

private:
	ALLEGRO_VOICE * mainVoice;
	ALLEGRO_MIXER * mainMixer;

	std::vector<rSoundInstance *> soundInstances;
	std::vector<rStreamInstance *> streamInstances;
};

