#pragma once
#include "rSoundInstance.h"
#include "rStreamInstance.h"
#include "rVoiceoverInstance.h"
#define SNDINIT if(!this->Initialized) return;
#define SNDINIT_PTR if(!this->Initialized) return NULL;
class rSound
{
public:
	rSound();
	~rSound();

	bool Initialized = false;

	bool Init();

	void Frame();
	
	void PlayMusic(char * musName);
	void PlaySample(char * sndName);
	float * PlayVoiceover(char * voxName, bool stopPrevious = true);

private:
	ALLEGRO_VOICE * mainVoice;
	ALLEGRO_MIXER * mainMixer;

	std::vector<rSoundInstance *> soundInstances;
	std::vector<rStreamInstance *> streamInstances;
	std::vector<rVoiceoverInstance *> voiceoverInstances;
};

