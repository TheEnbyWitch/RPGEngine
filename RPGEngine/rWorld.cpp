#include "stdafx.h"
#include "rWorld.h"


rWorld::rWorld()
{
}


rWorld::~rWorld()
{
}

void rWorld::Frame()
{
	timeOfDay += 0.00005;
	if (timeOfDay > 1) timeOfDay -= 1;
	currentTOD = ProcessTOD();
}

void rWorld::Draw()
{
	int currentLayer = 0;
	rMap *loadedMap = NULL;
	for (int i = 0; i < loadedMaps.size(); i++)
	{
		if (loadedMaps[i].isActive)
		{
			loadedMap = &loadedMaps[i];
			break;
		}
	}
	if (loadedMap == NULL) abort_game("Tried to draw NULL map!");
	al_hold_bitmap_drawing(true);
	for (int i = 0; i < loadedMap->maxLayers; i++)
	{
		loadedMap->Draw(i);
		if (i == 1) player.Draw();
	}
	al_hold_bitmap_drawing(false);
	if(loadedMap->maxLayers <= 1)
		player.Draw();
}

void rWorld::LoadMap(char * name)
{
	char path[256];
	sprintf(path, "maps/%s.tmx", name);
	rMap result;
	result.map = NLLoadTmxMap(ReadMap(path));
	memcpy(result.name, name, 16);
	rpge_printf("[rWorld] Loaded map %s\n", path);
	loadedMaps.push_back(result);
	loadedMaps[loadedMaps.size() - 1].ProcessMap();
	loadedMaps[loadedMaps.size() - 1].Optimize();
}

double GetMod(double v, double mod)
{
	while (v > mod)
	{
		v -= mod;
	}
	return v;
}

ALLEGRO_COLOR MixColors(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, double weight)
{
	ALLEGRO_COLOR res;
	res = al_map_rgb(
		c1.r*(1-weight)*255 + c2.r*(weight) * 255,
		c1.g*(1 - weight) * 255 + c2.g*(weight) * 255,
		c1.b*(1 - weight) * 255 + c2.b*(weight) * 255
		);
	return res;
}

ALLEGRO_COLOR rWorld::GetColorTint()
{
	return currentTOD;
}

char * rWorld::ReadMap(char * path)
{
	ALLEGRO_FILE * map;
	map = al_fopen(path, "rb");
	if (map == NULL)
	{
		char error[512];
		sprintf(error, "Map doesn't exist!\n%s", path);
		abort_game(error);
		return nullptr;
	}
	int size;
	al_fseek(map, 0, ALLEGRO_SEEK_END);
	size = al_ftell(map);
	al_fseek(map, 0, ALLEGRO_SEEK_SET);
	char *script = (char *)malloc(size+1);
	al_fread(map, script, size);
	script[size] = '\0';
	return script;
}

ALLEGRO_COLOR rWorld::ProcessTOD()
{
	ALLEGRO_COLOR res;
	int colorPos = 0;
	if (timeOfDay > 0.25)	colorPos = 1;
	if (timeOfDay > 0.5)	colorPos = 2;
	if (timeOfDay > 0.75)	colorPos = 3;
	double mix = GetMod(timeOfDay, 0.25)*4.0;
	ALLEGRO_COLOR c1 = timeOfDayTint[colorPos];
	ALLEGRO_COLOR c2 = timeOfDayTint[(colorPos + 1 > 3 ? colorPos - 3 : colorPos + 1)];
	res = MixColors(c1, c2, mix);
	return res;
}
