#pragma once
#include"stdafx.h"
class rMap
{
public:
	rMap();
	~rMap();

	void ProcessMap();
	int maxLayers = 0;
	char name[16];
	NLTmxMap *map;
	std::vector<rTile> tiles;
	std::vector<rEntity> entities;
	bool isActive = true;
	bool wasProcessed = false;
};

