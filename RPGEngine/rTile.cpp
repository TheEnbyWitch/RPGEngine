#include "stdafx.h"
#include "rTile.h"


rTile::rTile()
{
}

rTile::rTile(int x, int y, int layer, char* name)
{
	PositionX = x * 32;
	PositionY = y * 32;
	Layer = layer;
	strcpy(pathToImage, name);
}

rTile::~rTile()
{
}

void rTile::Draw()
{
	ALLEGRO_BITMAP* bm = gBitmap.GetBitmap(pathToImage);
	int *pos = GetScreenPos();
	al_draw_tinted_bitmap(bm, gWorld.GetColorTint(), pos[0], pos[1], NULL);
}
