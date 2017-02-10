#include "stdafx.h"
#include "rMap.h"


rMap::rMap()
{
}


rMap::~rMap()
{
}

// CALL THIS AFTER TMX Map was loaded!
void rMap::ProcessMap()
{
	rpge_printf("Processing map %s\n", name);
	int layers = map->layers.size();
	
	for (int i = 0; i < layers; i++)
	{
		int tiles = map->layers[i]->height * map->layers[i]->width;
		maxLayers = layers;
		for (int o = 0; o < tiles; o++)
		{
			if (map->layers[i]->data[o] == 0) continue;
			rTile tile;
			tile.Layer = i;
			int firstGid = 0;
			for (int p = 0; p < map->tilesets.size(); p++)
			{
				if (map->tilesets[p]->firstGid <= map->layers[i]->data[o] && map->tilesets[p]->firstGid > firstGid)
					firstGid = map->tilesets[p]->firstGid;
			}
			tile.tileID = map->layers[i]->data[o]-firstGid;
			tile.PositionX = (o % map->layers[i]->width-1)*32;
			tile.pX = (o % map->layers[i]->width - 1);
			int Yofs = 0;
			int oo = o;
			while (oo > map->layers[i]->width)
			{
				oo -= map->layers[i]->width-1;
				Yofs++;
			}
			tile.PositionY = Yofs * 32;
			tile.pY = Yofs;
			for (int p = 0; p < map->tilesets.size(); p++)
			{
				if (map->tilesets[p]->firstGid == firstGid)
				{
					char path[256];
					tile.columns = map->tilesets[p]->columns;
					memcpy(path, map->tilesets[p]->filename.c_str(), 256);
					sprintf(tile.pathToImage, "maps/%s", path);
					break;
				}
			}
			tile.Cache();
			this->tiles.push_back(tile);
		}
	}
	gScript.ExecuteLevelScript(name);
	rpge_printf("Map %s has been fully processed and is ready to use in game!\n", name);
	wasProcessed = true;
}

void rMap::Optimize()
{
	rTile **** _tiles = new rTile ***[maxLayers];
	for (int o = 0; o < maxLayers; o++)
	{
		_tiles[o] = new rTile **[__height];
		for (int i = 0; i < __height; i++)
		{
			_tiles[o][i] = new rTile *[__width];
		}
	}
	for (int i = 0; i < tiles.size(); i++)
	{
		_tiles[tiles[i].Layer][tiles[i].pY][tiles[i].pX] = &tiles[i];
	}
	int chunkCount = (((__height / 32) + 1)*((__width / 32) + 1));

}
