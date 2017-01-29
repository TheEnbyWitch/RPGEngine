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
	int pos[2];
	pos[0] = PositionX * 32.0f;
	pos[1] = PositionY * 32.0f;
	return pos;
}

int * rEntity::GetSourcePos()
{
	int spos[2];
	spos[1] = 32 * Direction;
	spos[0] = 32 * animationFrame;
	return spos;
}

rScaledRegion_t rEntity::GetScaledRegion()
{
	rScaledRegion_t result;
	int *pos = GetScreenPos();
	int *spos = GetSourcePos();
	result.destinationX = pos[0];
	result.destinationY = pos[1];
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
	pathToImage = path;
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
}

void rEntity::Move(int x, int y)
{
	if (!isMoving)
	{
		TargetX = PositionX + x;
		TargetY = PositionY + y;
	}
}
