#pragma once
#include "stdafx.h"
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

	ALLEGRO_BITMAP* GetBitmap(char* name);
};

