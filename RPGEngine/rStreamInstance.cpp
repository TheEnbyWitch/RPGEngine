#include "stdafx.h"
#include "rStreamInstance.h"

#include "RapidXML\rapidxml.hpp"


rStreamInstance::rStreamInstance()
{
}

rStreamInstance::rStreamInstance(char * filename)
{
	strcpy(name, filename);
	sprintf(xmlName, "%s.xml", filename);
}

rStreamInstance::rStreamInstance(const char * filename)
{
	strcpy(name, filename);
	sprintf(xmlName, "%s.xml", filename);
}

rStreamInstance::~rStreamInstance()
{
}

void rStreamInstance::Init()
{
	audioStream = al_load_audio_stream(name, 8, 8192);
	
	ALLEGRO_FILE * xml;
	xml = al_fopen(xmlName, "rb");
	if (xml)
	{
		char * buf = new char[al_fsize(xml) + 1];
		al_fread(xml, buf, al_fsize(xml));
		buf[al_fsize(xml)] = '\0';
		al_fclose(xml);

		rapidxml::xml_document<> x;
		x.parse<0>(buf);
		fadeInLength = atof(x.first_node("streamEntry")->first_attribute("fadeIn")->value());
		fadeOutLength = atof(x.first_node("streamEntry")->first_attribute("fadeOut")->value());
	}

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
		if (fadeInLength != 0.0)
			currentGain = __min(1.0, currentGain + (1.0 / 60.0) / fadeInLength);
		else
			currentGain = 1.0;
		al_set_audio_stream_gain(audioStream, currentGain);
		if (currentGain >= 1.0)
		{
			state = STREAM_PLAYING;
		}
		break;
	case STREAM_FADING_OUT:
		if (fadeOutLength != 0.0)
			currentGain = __max(0.0, currentGain - (1.0 / 60.0) / fadeOutLength);
		else
			currentGain = 0.0;
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
