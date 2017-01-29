#include "stdafx.h"
#include "rEntity.h"


rEntity::rEntity()
{
}


rEntity::~rEntity()
{
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
}

void rEntity::Draw()
{
	ALLEGRO_BITMAP * bm = gBitmap.GetBitmap(pathToImage);
	rScaledRegion_t r = GetScaledRegion();
	al_draw_scaled_bitmap(bm, r.sourceX, r.sourceY, r.sourceW, r.sourceH, r.destinationX, r.destinationY, r.destinationW, r.destinationH, NULL);
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

void rEntity::Move(int x, int y)
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
	}
}
