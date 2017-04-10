#include "stdafx.h"
#include "rSoundInstance.h"
#include "rStreamInstance.h"
#include "rSound.h"

rSound gSound;
char currentMus[256];

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
	voxMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	al_set_default_mixer(mainMixer);
	al_attach_mixer_to_voice(mainMixer, mainVoice);
	Initialized = true;
	return true;
}

void rSound::Frame()
{
	SNDINIT;
	for (auto si : streamInstances)
	{
		si->Frame();
	}
}

void rSound::PlayMusic(char * musName)
{
	SNDINIT;
	strcpy(currentMus, musName);
	bool found = false;
	for (auto si : streamInstances)
	{
		if (strcmp(musName, si->name) == 0)
		{
			si->Start();
			found = true;
		}
		else {
			si->FadeOut();
		}
	}
	if (found) return;
	rStreamInstance *t = new rStreamInstance(musName);
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

float * rSound::PlayVoiceover(char * voxName, bool stopPrevious)
{
	SNDINIT_PTR;
	for (auto si : voiceoverInstances)
	{
		if (strcmp(voxName, si->name) == 0)
		{
			si->Play();
		}
		else {
			if (stopPrevious)
				si->Stop();
		}
	}
	rVoiceoverInstance *t = new rVoiceoverInstance(voxName);
	t->Init();
	t->AttachToMixer(mainMixer);
	t->Play();
	voiceoverInstances.push_back(t);

	return t->GetVoiceIntensityPtr();
}

