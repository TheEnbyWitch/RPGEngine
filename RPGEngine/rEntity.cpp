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
	pos[0] = PositionX * 32;
	pos[1] = PositionY * 32;
	return pos;
}

void rEntity::SetImage(char * path)
{
	pathToImage = path;
}

void rEntity::Draw()
{
	ALLEGRO_BITMAP * bm = gBitmap.GetBitmap(pathToImage);
	int* pos = GetScreenPos();
	al_draw_bitmap(bm, pos[0], pos[1], NULL);
}
