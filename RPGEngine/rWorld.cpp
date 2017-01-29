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
	timeOfDay += 0.005;
	if (timeOfDay > 1) timeOfDay -= 1;
}

void rWorld::Draw()
{
	int currentLayer = 0;
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
	ALLEGRO_COLOR res;
	int colorPos = 0;
	if (timeOfDay > 0.25)	colorPos = 1;
	if (timeOfDay > 0.5)	colorPos = 2;
	if (timeOfDay > 0.75)	colorPos = 3;
	double mix = GetMod(timeOfDay, 0.25)*4.0;
	ALLEGRO_COLOR c1 = timeOfDayTint[colorPos];
	ALLEGRO_COLOR c2 = timeOfDayTint[(colorPos+1 > 3 ? colorPos-3 : colorPos+1)];
	res = MixColors(c1, c2, mix);
	return res;
}

void rWorld::CreateEntity(rEntityClass classname)
{
	rEntity newEnt;
	newEnt.classn = classname;
	entities.push_back(newEnt);
}
