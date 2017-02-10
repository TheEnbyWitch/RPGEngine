#pragma once
#include"stdafx.h"

struct rMapChunk
{
	ALLEGRO_BITMAP * resultBitmap;
	int posX;
	int posY;
};

class rMap
{
public:
	rMap();
	~rMap();

	void ProcessMap();
	void Optimize();
	int maxLayers = 0;
	char name[16];
	NLTmxMap *map;
	std::vector<rTile> tiles;
	std::vector<rEntity> entities;

	rMapChunk cachedChunks[16];

	bool isActive = true;
	bool wasProcessed = false;
	bool isOptimized = false;
};

