#include "stdafx.h"
#include "rBitmap.h"


rBitmap::rBitmap()
{
}


rBitmap::~rBitmap()
{
}

ALLEGRO_BITMAP * rBitmap::GetBitmap(char * fname, bool getEmissive, bool dontAddExtension)
{
	if (strcmp(fname, "null") == 0) return NULL;
	char name[256];
	if (!dontAddExtension) 
	{
		if (getEmissive)
			sprintf(name, "%s_e.tga", fname);
		else
			sprintf(name, "%s.tga", fname);
	}
	else {
		sprintf(name, "%s", fname);
	}
	for (int i = 0; i < entries.size(); i++)
	{
		if (strcmp(name, entries[i].name) == 0)
		{
			return ReturnValidImage(entries[i].bitmap);
		}
	}
	rBitmapEntry_t entry;
	memcpy(entry.name, name, 256);
	entry.bitmap = al_load_bitmap(name);
	entries.push_back(entry);
	return ReturnValidImage(entries[entries.size() - 1].bitmap);
}

ALLEGRO_BITMAP * rBitmap::GetDefaultBitmap()
{
	if (this->default == NULL)
		this->default = al_load_bitmap("default.tga");

	if (this->default == NULL)
		abort_game("Couldn't load default texture!");
	return this->default;
}

ALLEGRO_BITMAP * rBitmap::ReturnValidImage(ALLEGRO_BITMAP * bm)
{
#ifdef BITMAP_VERBOSE
	if (bm == NULL)
		abort_game("Missing bitmap!");
#endif
	if (bm == NULL)
		return GetDefaultBitmap();
	else
		return bm;
}
