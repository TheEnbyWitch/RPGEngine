#include "stdafx.h"

#ifndef __RPG_RMAP
#define __RPG_RMAP

struct rMapChunk
{
	ALLEGRO_BITMAP * resultBitmap;
	int posX;
	int posY;
	int layer;
};

class rMap
{
public:
	rMap();
	~rMap();

	void ProcessMap();
	void Optimize();
	void Draw(int layer);
	int maxLayers = 0;
	char name[16];
	NLTmxMap *map;

	std::vector<rTile> tiles;
	rTile **** tilePointers; // [layer][y][x]

	std::vector<rEntity> entities;

	rMapChunk cachedChunks[64];
	int cachedChunkSize = 0;

	bool isActive = true;
	bool wasProcessed = false;
	bool isOptimized = false;
};

#endif