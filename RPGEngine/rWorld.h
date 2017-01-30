#pragma once
#include "stdafx.h"
#include "rEntity.h"
class rWorld
{
public:
	rWorld();
	~rWorld();

	double timeOfDay = 0.5;
#define al_map_rgb(r,g,b) ALLEGRO_COLOR{r/255.0f, g/255.0f, b/255.0f, 1.0f}
#ifdef TEST_EMISSIVE
	ALLEGRO_COLOR timeOfDayTint[4] = {al_map_rgb(0,0,0),al_map_rgb(0,0,0),al_map_rgb(0,0,0),al_map_rgb(255,255,255) };//  al_map_rgb(0,32,114), al_map_rgb(255,191,132), al_map_rgb(255,255,255), al_map_rgb(255,191,132) };
#else
	ALLEGRO_COLOR timeOfDayTint[4] = { al_map_rgb(0,32,114), al_map_rgb(255,191,132), al_map_rgb(255,255,255), al_map_rgb(255,191,132) };
#endif
#undef al_map_rgb
	void Frame();
	void Draw();
	void LoadMap(char* name);
	ALLEGRO_COLOR GetColorTint();
	
	std::vector<rMap> loadedMaps;

private:
	char* ReadMap(char * path);
};

