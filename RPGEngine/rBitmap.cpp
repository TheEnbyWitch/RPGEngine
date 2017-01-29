#include "stdafx.h"
#include "rBitmap.h"


rBitmap::rBitmap()
{
}


rBitmap::~rBitmap()
{
}

ALLEGRO_BITMAP * rBitmap::GetBitmap(char * name)
{
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
