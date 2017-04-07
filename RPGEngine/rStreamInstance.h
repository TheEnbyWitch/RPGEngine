#pragma once
class rStreamInstance
{
public:
	rStreamInstance();
	rStreamInstance(char * filename);
	~rStreamInstance();

	void Init();
	void Start();

	void SetPlayMode(ALLEGRO_PLAYMODE playMode);
	bool AttachToMixer(ALLEGRO_MIXER * mixer);

	ALLEGRO_AUDIO_STREAM * GetStreamPtr();

	char name[256];

private:
	ALLEGRO_AUDIO_STREAM * audioStream;
	ALLEGRO_MIXER * attachedMixer;
};

