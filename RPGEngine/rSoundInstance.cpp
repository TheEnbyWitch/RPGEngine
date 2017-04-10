#include "stdafx.h"
#include "rSoundInstance.h"


rSoundInstance::rSoundInstance()
{
}

rSoundInstance::rSoundInstance(char * filename)
{
	strcpy(name, filename);
}


rSoundInstance::~rSoundInstance()
{
}

void rSoundInstance::Init()
{
	sample = al_load_sample(name);
	sampleInstance = al_create_sample_instance(sample);
}

bool rSoundInstance::Play()
{
	return al_play_sample_instance(sampleInstance);
}

bool rSoundInstance::AttachToMixer(ALLEGRO_MIXER * mixer)
{
	return al_attach_sample_instance_to_mixer(sampleInstance, mixer);
}

ALLEGRO_SAMPLE * rSoundInstance::GetSamplePtr()
{
	return sample;
}

ALLEGRO_SAMPLE_INSTANCE * rSoundInstance::GetSampleInstancePtr()
{
	return sampleInstance;
}
