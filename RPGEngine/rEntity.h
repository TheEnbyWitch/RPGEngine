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

	int Level = 1;
	float PositionX = 0;
	float PositionY = 0;
	float Speed = 0.05;
	rEntityDirection Direction = ENT_DIRECTION_DOWN;
	int animationFrame = 0;
	bool isMoving = false;
	char* pathToImage;
	rEntityClass classn;

	int* GetScreenPos();
	int* GetSourcePos();
	rScaledRegion_t GetScaledRegion();
	void SetImage(char * path);
	void Draw();
	void Frame();
	void Move(int x, int y);

private:
	float TargetX = 0;
	float TargetY = 0;
};

