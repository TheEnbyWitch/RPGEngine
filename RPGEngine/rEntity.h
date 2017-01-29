#pragma once
#include "stdafx.h"

typedef enum {
	ENTITY_ACTOR,
	ENTITY_OBJECT,
	ENTITY_INTERACTABLE
} rEntityClass;

typedef enum {
	ENT_DIRECTION_DOWN,
	ENT_DIRECTION_LEFT,
	ENT_DIRECTION_RIGHT,
	ENT_DIRECTION_UP
} rEntityDirection;

class rEntity
{
public:
	rEntity();
	~rEntity();

	int Layer = 1;
	int Level = 1;
	int PositionX = 0;
	int PositionY = 0;
	int Speed = 2;
	rEntityDirection Direction = ENT_DIRECTION_DOWN;
	int animationFrame = 0;
	bool isMoving = false;
	char pathToImage[256];
	rEntityClass classn;

	int* GetScreenPos();
	int* GetSourcePos();
	rScaledRegion_t GetScaledRegion();
	void SetImage(char * path);
	void Draw();
	void Frame();
	void Move(int x, int y);

private:
	int TargetX = 0;
	int TargetY = 0;
};

