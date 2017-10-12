#pragma once
#include "rEntity.h"
#include "rTile.h"
#include "rMap.h"

enum rCollisionFlags {
	COLLISION_PASSABLE		= 1 << 0,
	COLLISION_SOLID			= 1 << 1
};


class rCollisionData
{
public:
	rCollisionData();
	~rCollisionData();

	int ** collisionFlags;
	bool collisionInitialized = false;

	void BuildCollision(rMap * mapPtr);
};

