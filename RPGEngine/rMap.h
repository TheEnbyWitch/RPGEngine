#pragma once
#include"stdafx.h"
class rMap
{
public:
	rMap();
	~rMap();

	void ProcessMap();

	char name[16];
	NLTmxMap *map;
	std::vector<rTile> tiles;
	bool isActive = false;
};

