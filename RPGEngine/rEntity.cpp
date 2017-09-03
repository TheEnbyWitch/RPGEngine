#include "stdafx.h"
#include "rEntity.h"

unsigned int entID = 0;
rEntityTable entityList;

rEntity::rEntity()
{
}


rEntity::~rEntity()
{
	if(weakRefFlag != NULL)
		weakRefFlag->Set(true);
}

int rEntity::AddRef()
{
	return ++refCount;
}

int rEntity::Release()
{
	if (--refCount == 0)
	{
		delete this;
		return 0;
	}
	return refCount;
}

asILockableSharedBool *rEntity::GetWeakRefFlag()
{
	if (!weakRefFlag)
	{
		weakRefFlag = asCreateLockableSharedBool();
	}
	return weakRefFlag;
}

void rEntity::Activate()
{
	Activate(true);
}

void rEntity::Activate(bool addToCollection)
{
	if(addToCollection) entityList.push_back(this);
	sprintf(uniqueID, "ENT%06d\0", entID);
	rpge_printf("[rEntity] Entity with ID %s was created\n", uniqueID);
	entID++;
	wasCreated = true;
	PositionX = 0;
	PositionY = 0;
	strcpy(currentLevel, player.currentLevel);
}

void rEntity::Activate(char* id, bool addToCollection)
{
	if (addToCollection) entityList.push_back(this);
	sprintf(uniqueID, "%s", id);
	rpge_printf("[rEntity] Entity with ID %s was created\n", uniqueID);
	entID++;
	wasCreated = true;
	PositionX = 0;
	PositionY = 0;
	strcpy(currentLevel, player.currentLevel);
}

void rEntity::ActivateC(char* id)
{
	if (true) entityList.push_back(this);
	sprintf(uniqueID, "%s", id);
	rpge_printf("[rEntity] Entity with ID %s was created\n", uniqueID);
	entID++;
	wasCreated = true;
	PositionX = 0;
	PositionY = 0;
	strcpy(currentLevel, player.currentLevel);
}

void rEntity::Activate(int x, int y, char* level, bool addToCollection)
{
	if (addToCollection) entityList.push_back(this);
	sprintf(uniqueID, "ENT%06d\0", entID);
	rpge_printf("[rEntity] Entity with ID %s was created\n", uniqueID);
	entID++;
	wasCreated = true;
	PositionX = x;
	PositionY = y;
	strcpy(currentLevel, level);
}

void rEntity::Destroy()
{
	delete this;
}

rVector2 rEntity::GetScreenPos()
{
	WS
	rVector2 pos;
	pos.X = PositionX;
	pos.Y = PositionY;
	return pos;
}

rVector2 rEntity::GetSourcePos()
{
	WS
	rVector2 r;
	r.Y = 32 * Direction;
	r.X = 32 * (int)(animationFrame/6);
	return r;
}

rScaledRegion_t rEntity::GetScaledRegion()
{
	WS
	rScaledRegion_t result;
	rVector2 pos = GetScreenPos();
	rVector2 spos = GetSourcePos();
	result.destinationX = pos.X - player.cameraOffset.X;// -spos[0];
	result.destinationY = pos.Y - player.cameraOffset.Y;// -spos[1];
	result.destinationW = 32;
	result.destinationH = 32;
	result.sourceX = spos.X;
	result.sourceY = spos.Y;
	result.sourceW = 32;
	result.sourceH = 32;
	return result;
}

void rEntity::SetImage(char * path)
{
	WS
	strcpy(pathToImage, path);
	gBitmap.GetBitmap(pathToImage); // precache the images
	gBitmap.GetBitmap(pathToImage, true);
	isImageSet = true;
}

void rEntity::Draw()
{
	WS
	if (isImageSet)
	{
		ALLEGRO_BITMAP * bm = gBitmap.GetBitmap(pathToImage);
		ALLEGRO_BITMAP * embm = NULL;
		if (useEmissive) embm = gBitmap.GetBitmap(pathToImage, true);
		ALLEGRO_COLOR tint = gWorld.GetColorTint();
		rScaledRegion_t r = GetScaledRegion();
		al_draw_tinted_scaled_bitmap(bm, tint, r.sourceX, r.sourceY, r.sourceW, r.sourceH, r.destinationX, r.destinationY, r.destinationW, r.destinationH, NULL);
		if (embm != NULL && useEmissive) al_draw_scaled_bitmap(embm, r.sourceX, r.sourceY, r.sourceW, r.sourceH, r.destinationX, r.destinationY, r.destinationW, r.destinationH, NULL);
	}
}

void rEntity::Frame()
{
	WS

	gScript.EntThink(this);
	
	if (Direction > 3)
		Direction = ENT_DIRECTION_DOWN;
	if (Direction < 0)
		Direction = ENT_DIRECTION_UP;

	if (PositionX != TargetX)
		PositionX += Speed*(TargetX < PositionX ? -1 : 1);
	if (PositionY != TargetY)
		PositionY += Speed*(TargetY < PositionY ? -1 : 1);

	if (isMoving)
	{
		animationFrame++;
		if (animationFrame > 23)
			animationFrame = 0;
	}
	else
	{
		animationFrame = 0;
	}

	if (PositionX == TargetX && PositionY == TargetY)
		isMoving = false;
}

bool rEntity::Move(int x, int y)
{
	WS
	if (!isMoving)
	{
		if (y > 0) ChangeDirection(ENT_DIRECTION_DOWN);
		if (y < 0) ChangeDirection(ENT_DIRECTION_UP);
		if (x > 0) ChangeDirection(ENT_DIRECTION_RIGHT);
		if (x < 0) ChangeDirection(ENT_DIRECTION_LEFT);
		TargetX = PositionX + x*32;
		TargetY = PositionY + y*32;
		isMoving = true;
		return true;
	}
	return false;
}

void rEntity::Interact()
{
	gScript.EntInteract(this);
}

void rEntity::ChangeDirection(int targetDirection)
{
	if (targetDirection >= 0 && targetDirection < ENT_DIRECTION_MAX)
	{
		Direction = (rEntityDirection)targetDirection;
	}
}

rEntity * rEntity::SpawnEntity()
{
	if (entityList.GetEntCount() > 1023) abort_game("Too many entities");
	return new rEntity;
}

rVector2 rEntity::GetVectorForDirection(rEntityDirection dir)
{
	if (dir == ENT_DIRECTION_DOWN)
	{
		return rVector2(0, 1);
	}
	if (dir == ENT_DIRECTION_UP)
	{
		return rVector2(0, -1);
	}
	if (dir == ENT_DIRECTION_LEFT)
	{
		return rVector2(-1, 0);
	}
	if (dir == ENT_DIRECTION_RIGHT)
	{
		return rVector2(1, 0);
	}
	return rVector2(0,1); // return default if for some retarded reason a different ENT_DIRECTION was passed which should be impossible
}

rEntity *GetEntityById(string id)
{
	for (int i = 0; i < entityList.size(); i++)
	{
		if (strcmp(entityList[i]->uniqueID, id.c_str()) == 0) return entityList[i];
	}
	return NULL;
}

void RemoveEntity(rEntity * ent)
{
	entityList.RemoveEntity(ent);
}

int rEntityTable::GetEntCount()
{
	int res = 0;
	for (int i = 0; i < this->size(); i++)
	{
		if ((*this)[i] != NULL) res++;
	}
	return res;
}

void rEntityTable::RemoveEntity(rEntity * ent)
{
	for (int i = 0; i < this->size(); i++)
	{
		if ((*this)[i] == ent)
		{
			ent->Destroy();
			erase(begin() + i);
		}
	}
}
