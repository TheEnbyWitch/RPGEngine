#pragma once
#include "stdafx.h"

typedef enum {
	ENTITY_ACTOR,
	ENTITY_OBJECT,
	ENTITY_INTERACTABLE
} rEntityClass;

class rEntity
{
public:
	rEntity();
	~rEntity();

	int Level = 1;
	int PositionX;
	int PositionY;
	char* pathToImage;
	rEntityClass classn;

	int* GetScreenPos();
	void SetImage(char * path);
	void Draw();
};

