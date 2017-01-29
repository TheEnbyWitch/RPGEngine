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

void rWorld::CreateEntity(rEntityClass classname)
{
	rEntity newEnt;
	newEnt.classn = classname;
	entities.push_back(newEnt);
}
