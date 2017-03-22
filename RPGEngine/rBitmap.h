#include "stdafx.h"

#ifndef __RPG_RBITMAP
#define __RPG_RBITMAP

typedef struct rBitmapEntry_s {
	char name[256];
	ALLEGRO_BITMAP * bitmap;
} rBitmapEntry_t;

class rBitmap
{
public:
	rBitmap();
	~rBitmap();
	std::vector<rBitmapEntry_t> entries;
	ALLEGRO_BITMAP* default;

	ALLEGRO_BITMAP* GetBitmap(char* fname, bool getEmissive = false, bool dontAddExtension = false);
	ALLEGRO_BITMAP* GetDefaultBitmap();

private:
	ALLEGRO_BITMAP* ReturnValidImage(ALLEGRO_BITMAP* bm);
};

#endif