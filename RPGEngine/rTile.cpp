#include "stdafx.h"
#include "rTile.h"


rTile::rTile()
{
}

rTile::rTile(int x, int y, char* name)
{
	PositionX = x * 32;
	PositionY = y * 32;
	strcpy(pathToImage, name);
}

rTile::~rTile()
{
}

void rTile::Draw()
{
	ALLEGRO_BITMAP* bm = gBitmap.GetBitmap(pathToImage);
	int *pos = GetScreenPos();
	//al_draw_bitmap(bm, pos[0], pos[1], NULL);
	al_draw_tinted_bitmap(bm, gWorld.GetColorTint(), pos[0], pos[1], NULL);
}
