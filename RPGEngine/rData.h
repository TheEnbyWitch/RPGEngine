#ifndef __RPG_RDATA
#define __RPG_RDATA

class rData
{
public:
	rData();
	~rData();

	int Init(char* savefile);

	template <class T>
	void RegisterSaveField(const char * name, T defaultValue);

private:
	uint8_t* rawSaveData;
	nlohmann::json data;
	std::vector<std::string> saveFields;
	ALLEGRO_FILE * _savefile_ptr = 0;
};


// Script wrappers
void RegisterSaveFieldInt(char * name, int defaultVal);
void RegisterSaveFieldFloat(char * name, float defaultVal);
void RegisterSaveFieldString(char * name, char* defaultVal);

#endif
