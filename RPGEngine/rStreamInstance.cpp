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
	audioStream = al_load_audio_stream(name, 4, 2048);
	
	al_set_audio_stream_playing(audioStream, false);
	al_set_audio_stream_playmode(audioStream, ALLEGRO_PLAYMODE_LOOP);
}

void rStreamInstance::Start()
{
	al_set_audio_stream_playing(audioStream, true);
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
