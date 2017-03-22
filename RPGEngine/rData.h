#ifndef __RPG_RDATA
#define __RPG_RDATA

class rData
{
public:
	rData();
	~rData();

	int Init(char* savefile);


private:
	uint8_t* rawSaveData;
	nlohmann::json data;
	ALLEGRO_FILE * _savefile_ptr = 0;
};

#endif