#pragma once
class rSoundInstance
{
public:
	rSoundInstance();
	rSoundInstance(char * filename);
	~rSoundInstance();

	void Init();
	bool Play();

	bool AttachToMixer(ALLEGRO_MIXER * mixer);

	ALLEGRO_SAMPLE * GetSamplePtr();
	ALLEGRO_SAMPLE_INSTANCE * GetSampleInstancePtr();

	char name[256];
private:
	ALLEGRO_SAMPLE * sample;
	ALLEGRO_SAMPLE_INSTANCE * sampleInstance;
};

