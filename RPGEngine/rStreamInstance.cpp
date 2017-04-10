#include "stdafx.h"
#include "rStreamInstance.h"


rStreamInstance::rStreamInstance()
{
}

rStreamInstance::rStreamInstance(char * filename)
{
	strcpy(name, filename);
}


rStreamInstance::~rStreamInstance()
{
}

void rStreamInstance::Init()
{
	audioStream = al_load_audio_stream(name, 8, 8192);
	
	al_set_audio_stream_playing(audioStream, false);
	al_set_audio_stream_playmode(audioStream, ALLEGRO_PLAYMODE_LOOP);
}

void rStreamInstance::Frame()
{
	switch (state)
	{
	case STREAM_IDLE:
		break;
	case STREAM_FADING_IN:
		currentGain = __min(1.0, currentGain + (1.0 / 60.0) * fadeInLength);
		al_set_audio_stream_gain(audioStream, currentGain);
		if (currentGain >= 1.0)
		{
			state = STREAM_PLAYING;
		}
		break;
	case STREAM_FADING_OUT:
		currentGain = __max(0.0, currentGain - (1.0 / 60.0) * fadeInLength);
		al_set_audio_stream_gain(audioStream, currentGain);
		if (currentGain <= 0.0)
		{
			Stop();
		}
		break;
	case STREAM_PLAYING:
		break;
	}
}

void rStreamInstance::Start()
{
	if (state == STREAM_FADING_IN || state == STREAM_PLAYING) return;
	al_set_audio_stream_gain(audioStream, 0.0);
	al_set_audio_stream_playing(audioStream, true);
	state = STREAM_FADING_IN;
}

void rStreamInstance::Stop()
{
	al_set_audio_stream_playing(audioStream, false);
	state = STREAM_IDLE;
}

void rStreamInstance::FadeOut()
{
	if (state == STREAM_FADING_OUT || state == STREAM_IDLE) return;
	state = STREAM_FADING_OUT;
}

void rStreamInstance::SetPlayMode(ALLEGRO_PLAYMODE playMode)
{
	al_set_audio_stream_playmode(audioStream, playMode);
}

bool rStreamInstance::AttachToMixer(ALLEGRO_MIXER * mixer)
{
	return al_attach_audio_stream_to_mixer(audioStream, mixer);
}

ALLEGRO_AUDIO_STREAM * rStreamInstance::GetStreamPtr()
{
	return audioStream;
}
