#include "stdafx.h"
#include "rData.h"

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
	_savefile_ptr = al_fopen(va("%s.rsf", savefile), "rb");
	if(_savefile_ptr != 0)
	{
		// read savefile and copy data
		_savefile_ptr = al_fopen(va("%s.rsf", savefile), "wb");
		return 1;
	}
	else {
		// create new savefile
		_savefile_ptr = al_fopen(va("%s.rsf", savefile), "wb");
		if (_savefile_ptr == 0) abort_game("Couldn't initialize save system!");
		al_fwrite(_savefile_ptr, "rpgsf", 5);
	}
	return 0;
}