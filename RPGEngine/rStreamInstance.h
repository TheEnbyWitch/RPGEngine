#pragma once

typedef enum {
	STREAM_IDLE,
	STREAM_FADING_IN,
	STREAM_PLAYING,
	STREAM_FADING_OUT
} rStreamState_e;

class rStreamInstance
{
public:
	rStreamInstance();
	rStreamInstance(char * filename);
	~rStreamInstance();

	void Init();

	void Frame();

	void Start();
	void Stop();

	void FadeOut();

	void SetPlayMode(ALLEGRO_PLAYMODE playMode);
	bool AttachToMixer(ALLEGRO_MIXER * mixer);

	ALLEGRO_AUDIO_STREAM * GetStreamPtr();

	char name[256];
	char xmlName[256];
	double fadeOutLength = 2;
	double fadeInLength = 2;
	double currentGain = 0.0;

	rStreamState_e state = STREAM_IDLE;

private:
	ALLEGRO_AUDIO_STREAM * audioStream;
	ALLEGRO_MIXER * attachedMixer;
};

