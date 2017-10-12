#include "stdafx.h"

#ifndef __RPG_RENTITY
#define __RPG_RENTITY

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
	ENT_DIRECTION_UP,
	ENT_DIRECTION_MAX
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
	bool useEmissive = false;
	char pathToImage[256];
	//char pathToEmmisiveLayer[256] = "null";
	char currentLevel[32];
	rEntityClass classn;

	int AddRef();
	int Release();

	asILockableSharedBool * GetWeakRefFlag();

	bool wasCreated = false;
	void Activate();
	void Activate(bool addToCollection);
	void ActivateC(char* id);
	void Activate(char* id, bool addToCollection);
	void Activate(int x, int y, char* level, bool addToCollection = true);
	void Destroy();

	rVector2 GetScreenPos();
	rVector2 GetSourcePos();
	rScaledRegion_t GetScaledRegion();
	void SetImage(char * path);
	void Draw();
	void Frame();

	bool Move(int x, int y);
	void Teleport(int x, int y);
	void Interact();
	void ChangeDirection(int targetDirection);

	// static
	static rEntity *SpawnEntity();
	static rVector2 GetVectorForDirection(rEntityDirection dir);
	
	// script
	//bool hasThinkFunc = true;
	asIScriptContext *thinkContext = 0;
	asIScriptFunction *thinkFunc = 0;

	asIScriptContext *interactContext = 0;
	asIScriptFunction *interactFunc = 0;

private:
	int TargetX = 0;
	int TargetY = 0;

protected:
	int refCount;
	asILockableSharedBool *weakRefFlag;
};

rEntity *GetEntityById(string id);

typedef std::vector<rEntity *> rEntityVector;

class rEntityTable : public rEntityVector
{
public:
	int GetEntCount();
	void RemoveEntity(rEntity * ent);
};

void RemoveEntity(rEntity * ent);

extern rEntityTable entityList;

#endif