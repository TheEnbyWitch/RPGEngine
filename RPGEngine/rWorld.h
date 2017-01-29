#pragma once
#include "stdafx.h"
#include "rEntity.h"
class rWorld
{
public:
	rWorld();
	~rWorld();

	double timeOfDay = 0.5;
	ALLEGRO_COLOR timeOfDayTint[4] = { al_map_rgb(0,32,114), al_map_rgb(255,255,255), al_map_rgb(255,191,132), al_map_rgb(0,32,114) };

	void Frame();
	
	std::vector<rEntity> entities;
	void CreateEntity(rEntityClass classname);
};

