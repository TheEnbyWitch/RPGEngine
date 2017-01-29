#pragma once
#include "stdafx.h"
#include "rEntity.h"
class rWorld
{
public:
	rWorld();
	~rWorld();

	std::vector<rEntity> entities;
	void CreateEntity(rEntityClass classname);
};

