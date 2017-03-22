#include "stdafx.h"
#include "rData.h"

using nlohmann::json;

rData::rData()
{
}


rData::~rData()
{
	if (_savefile_ptr != 0) al_fclose(_savefile_ptr);
}

int rData::Init(char * savefile)
{
	// 0 = new savefile
	// 1 = savefile exists
	this->data["game"] = "RPGE";
	_savefile_ptr = al_fopen(va("playerdata/%s.rsf", savefile), "rb");
	if(al_filename_exists(va("playerdata/%s.rsf", savefile)) && al_fsize(_savefile_ptr) >= 5)
	{
		// read savefile and copy data
		int svsize = al_fsize(_savefile_ptr) - 5;
		rawSaveData = new uint8_t[svsize];
		uint8_t * magic = new uint8_t[6];
		al_fread(_savefile_ptr, magic, 5); magic[5] = '/0';
		al_fread(_savefile_ptr, rawSaveData, svsize);
		this->data = json::from_cbor(std::vector<uint8_t>(rawSaveData, rawSaveData + svsize));
		rpge_printf("[rData] Loaded savefile: playerdata/%s.rsf (format: %s)\n", savefile, this->data["game"].get<string>().c_str());
		al_fclose(_savefile_ptr);
		return 1;
	}
	else {
		// create new savefile
#ifndef __ANDROID
		_savefile_ptr = al_fopen(va("%s.rsf", savefile), "wb");
#else
		_savefile_ptr = al_fopen(va("playerdata/%s.rsf", savefile), "wb");
#endif
		if (_savefile_ptr == 0) abort_game("Couldn't initialize save system!");
		al_fwrite(_savefile_ptr, "rpgsf", 5);
		auto jsnf = json::to_cbor(this->data);
		for (int i = 0; i < jsnf.size(); i++)
		{
			al_fwrite(_savefile_ptr, &((unsigned char)jsnf[i]), 1);
		}
		al_fclose(_savefile_ptr);
	}
	return 0;
}