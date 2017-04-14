#include "stdafx.h"
#include "rVoiceoverInstance.h"

void UpdateVoiceWrapper(void *buf, unsigned int samples, void *data)
{
	int *fbuf = (int *)buf;
	((rVoiceoverInstance *)data)->UpdateVoice(fbuf, samples, NULL);
}

rVoiceoverInstance::rVoiceoverInstance()
{
}

rVoiceoverInstance::rVoiceoverInstance(char * filename)
{
	strcpy(name, filename);
}

rVoiceoverInstance::rVoiceoverInstance(const char * filename)
{
	strcpy(name, filename);
}


rVoiceoverInstance::~rVoiceoverInstance()
{
}

void rVoiceoverInstance::Init()
{
	mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	sample = al_load_sample(name);
	sampleInstance = al_create_sample_instance(sample);
	al_attach_sample_instance_to_mixer(sampleInstance, mixer);
	al_set_mixer_postprocess_callback(mixer, UpdateVoiceWrapper, this);
}

bool rVoiceoverInstance::Play()
{
	return al_play_sample_instance(sampleInstance);
}

bool rVoiceoverInstance::Stop()
{
	return al_stop_sample_instance(sampleInstance);
}

bool rVoiceoverInstance::AttachToMixer(ALLEGRO_MIXER * targetMixer)
{
	return al_attach_mixer_to_mixer(mixer, targetMixer);
}

ALLEGRO_SAMPLE * rVoiceoverInstance::GetSamplePtr()
{
	return sample;
}

ALLEGRO_SAMPLE_INSTANCE * rVoiceoverInstance::GetSampleInstancePtr()
{
	return sampleInstance;
}

ALLEGRO_MIXER * rVoiceoverInstance::GetMixerPtr()
{
	return mixer;
}

float * rVoiceoverInstance::GetVoiceIntensityPtr()
{
	return &voiceIntensity;
}

void rVoiceoverInstance::UpdateVoice(int * buf, unsigned int samples, void * data)
{
	float sum_l = 0.0;
	float sum_r = 0.0;
	unsigned int i;

	(void)data;

	for (i = samples; i > 0; i--) {
		sum_l += abs(buf[0]) / (INT16_MAX - 1);
		sum_r += abs(buf[1]) / (INT16_MAX - 1);
		buf += 2;
	}

	float rms_l = sqrt(sum_l / samples);
	float rms_r = sqrt(sum_r / samples);
	voiceIntensity = (rms_l + rms_r);//sqrt(sum_l / samples) + sqrt(sum_r / samples);
	voiceIntensity = voiceIntensity / 255;
	//voiceIntensity = __max(0.0f, log10(voiceIntensity));
}
