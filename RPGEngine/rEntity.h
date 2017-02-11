#pragma once
#include "stdafx.h"

#define WS if(!wasCreated) abort_game("Tried to do an action on an uncreated entity");

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

	ALLEGRO_BITMAP* cachedBitmap;

	char uniqueID[10];
	int Layer = 1;
	int Level = 1;
	int PositionX = 0;
	int PositionY = 0;
	int Speed = 2;
	rEntityDirection Direction = ENT_DIRECTION_DOWN;
	int animationFrame = 0;
	bool isMoving = false;
	bool isImageSet = false;
	char pathToImage[256];
	char pathToEmmisiveLayer[256] = "null";
	rEntityClass classn;

	int AddRef();
	int Release();

	asILockableSharedBool * GetWeakRefFlag();

	bool wasCreated = false;
	void Create();

	int* GetScreenPos();
	int* GetSourcePos();
	rScaledRegion_t GetScaledRegion();
	void SetImage(char * path);
	void Draw();
	void Frame();
	bool Move(int x, int y);
	void Interact();

private:
	int TargetX = 0;
	int TargetY = 0;

protected:
	int refCount;
	asILockableSharedBool *weakRefFlag;
};

rEntity *GetEntityById(string id);
extern std::vector<rEntity *> entityList;
