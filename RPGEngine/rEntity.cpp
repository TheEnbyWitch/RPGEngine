#include "stdafx.h"
#include "rEntity.h"

unsigned int entID = 0;
std::vector<rEntity *> entityList;

rEntity::rEntity()
{
	entityList.push_back(this);
	sprintf(uniqueID, "ENT%06d\0", entID);
	entID++;
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

int * rEntity::GetScreenPos()
{
	int *pos = new int[2];
	pos[0] = PositionX;
	pos[1] = PositionY;
	return pos;
}

int * rEntity::GetSourcePos()
{
	int *spos = new int[2];
	spos[1] = 32 * Direction;
	spos[0] = 32 * (int)(animationFrame/6);
	return spos;
}

rScaledRegion_t rEntity::GetScaledRegion()
{
	rScaledRegion_t result;
	int *pos = GetScreenPos();
	int *spos = GetSourcePos();
	result.destinationX = pos[0];// -spos[0];
	result.destinationY = pos[1];// -spos[1];
	result.destinationW = 32;
	result.destinationH = 32;
	result.sourceX = spos[0];
	result.sourceY = spos[1];
	result.sourceW = 32;
	result.sourceH = 32;
	return result;
}

void rEntity::SetImage(char * path)
{
	strcpy(pathToImage, path);
	isImageSet = true;
}

void rEntity::Draw()
{
	if (isImageSet)
	{
		ALLEGRO_BITMAP * bm = gBitmap.GetBitmap(pathToImage);
		ALLEGRO_BITMAP * embm = gBitmap.GetBitmap(pathToImage, true);
		ALLEGRO_COLOR tint = gWorld.GetColorTint();
		rScaledRegion_t r = GetScaledRegion();
		al_draw_tinted_scaled_bitmap(bm, tint, r.sourceX, r.sourceY, r.sourceW, r.sourceH, r.destinationX, r.destinationY, r.destinationW, r.destinationH, NULL);
		if (embm != NULL) al_draw_scaled_bitmap(embm, r.sourceX, r.sourceY, r.sourceW, r.sourceH, r.destinationX, r.destinationY, r.destinationW, r.destinationH, NULL);
	}
}

void rEntity::Frame()
{
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
	if (!isMoving)
	{
		if (y > 0) Direction = ENT_DIRECTION_DOWN;
		if (y < 0) Direction = ENT_DIRECTION_UP;
		if (x > 0) Direction = ENT_DIRECTION_RIGHT;
		if (x < 0) Direction = ENT_DIRECTION_LEFT;
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

rEntity *GetEntityById(string id)
{
	for (int i = 0; i < entityList.size(); i++)
	{
		if (strcmp(entityList[i]->uniqueID, id.c_str()) == 0) return entityList[i];
	}
	return NULL;
}