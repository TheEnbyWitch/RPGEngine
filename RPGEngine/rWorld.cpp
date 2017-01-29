#include "stdafx.h"
#include "rWorld.h"


rWorld::rWorld()
{
}


rWorld::~rWorld()
{
}

void rWorld::CreateEntity(rEntityClass classname)
{
	rEntity newEnt;
	newEnt.classn = classname;
	entities.push_back(newEnt);
}
