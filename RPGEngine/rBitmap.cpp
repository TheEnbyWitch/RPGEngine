#include "stdafx.h"
#include "rBitmap.h"


rBitmap::rBitmap()
{
}


rBitmap::~rBitmap()
{
}

ALLEGRO_BITMAP * rBitmap::GetBitmap(char * fname, bool getEmissive)
{
	if (strcmp(fname, "null") == 0) return NULL;
	char name[256];
	if(getEmissive)
		sprintf(name, "%s_e.tga", fname);
	else
		sprintf(name, "%s.tga", fname);
	for (int i = 0; i < entries.size(); i++)
	{
		if (strcmp(name, entries[i].name) == 0)
		{
			return entries[i].bitmap;
		}
	}
	rBitmapEntry_t entry;
	memcpy(entry.name, name, 256);
	entry.bitmap = al_load_bitmap(name);
	entries.push_back(entry);
	return entries[entries.size() - 1].bitmap;
}
