#include "stdafx.h"
#include "rSoundInstance.h"
#include "rStreamInstance.h"
#include "rSound.h"

rSound gSound;


rSound::rSound()
{
}


rSound::~rSound()
{
}

bool rSound::Init()
{
	mainVoice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	al_set_default_voice(mainVoice);
	mainMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	al_set_default_mixer(mainMixer);
	al_attach_mixer_to_voice(mainMixer, mainVoice);
	Initialized = true;
	return true;
}

void rSound::PlayMusic()
{
	SNDINIT;
	for (auto si : streamInstances)
	{
		if (strcmp("test.flac", si->name) == 0)
		{
			si->Start();
			return;
		}
		else {
		}
	}
	rStreamInstance *t = new rStreamInstance("test.flac");
	t->Init();
	t->AttachToMixer(mainMixer);
	t->SetPlayMode(ALLEGRO_PLAYMODE_LOOP);
	t->Start();
	streamInstances.push_back(t);
}

void rSound::PlaySample(char * sndName)
{
	SNDINIT;
	for (auto si : soundInstances)
	{
		if (strcmp(sndName, si->name) == 0)
		{
			si->Play();
			return;
		}
	}
	rSoundInstance *t = new rSoundInstance(sndName);
	t->Init();
	t->AttachToMixer(mainMixer);
	t->Play();
	soundInstances.push_back(t);
}

