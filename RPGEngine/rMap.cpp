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
	rpge_printf("[rMap] Processing map %s\n", name);
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
	rpge_printf("[rMap] Map %s has been fully processed and is ready to use in game!\n", name);
	wasProcessed = true;
}

void rMap::Optimize()
{
	rTile **** _tiles = new rTile ***[maxLayers];
	for (int o = 0; o < maxLayers; o++)
	{
		_tiles[o] = new rTile **[map->height + 1];
		for (int i = 0; i < map->height + 1; i++)
		{
			_tiles[o][i] = new rTile *[map->width + 1];
			for (int p = 0; p < map->width + 1; p++)
			{
				_tiles[o][i][p] = NULL;
			}
		}
	}
	for (int i = 0; i < tiles.size(); i++)
	{
		//tiles[i].Cache();
		_tiles[tiles[i].Layer][tiles[i].pY][tiles[i].pX] = &tiles[i];
	}
	tilePointers = _tiles;
	/*
	int chunkColumns = (map->width / 32) + 1;
	int chunkRows = (map->height / 32) + 1;
	if (chunkColumns*chunkRows*maxLayers >= 64)
	{
		abort_game("Map too big for optimization!");
	}
	int chunks = 0;
	int bitmapsWereMade = 0;
	for (int u = 0; u < maxLayers; u++)
	{
		for (int i = 0; i < chunkColumns*chunkRows; i++)
		{
			int ofsX = i % chunkRows;
			int ofsY = i / chunkRows;
			ALLEGRO_BITMAP * targetbitmap = al_create_bitmap(32 * 32, 32 * 32);
			bitmapsWereMade++;
			ALLEGRO_BITMAP * original = al_get_target_bitmap();
			rMapChunk chnk;
			al_set_target_bitmap(targetbitmap);
			//for (int o = 0; o < maxLayers; o++)
			//{
				for (int p = 0; p < (map->height); p++)
				{
					for (int a = 0; a < (map->width); a++)
					{
						if (_tiles[u][p][a] != NULL)
							_tiles[u][p][a]->DrawToBitmap(ofsX * -32, ofsY * -32);
					}
				}

			//}
			al_set_target_bitmap(original);
			chnk.posX = ofsX;
			chnk.posY = ofsY;
			chnk.layer = u;
			chnk.resultBitmap = targetbitmap;
			cachedChunks[chunks] = chnk;
			chunks++;
		}
	}
	cachedChunkSize = chunks;
	*/
	rpge_printf("[rMap] %s was optimized", name);// with %d bitmaps\n", name, bitmapsWereMade);
	isOptimized = true;
}

void rMap::Draw(int layer)
{
	if (wasProcessed == false) abort_game("Tried to draw an unprocessed map!");
	if (isOptimized)
	{
		for (int i = max(0,(player.cameraOffset.X) / 32); i < ((980 + player.cameraOffset.X) / 32); i++)
		{
			for (int o = max(0, (player.cameraOffset.Y) / 32); o < ((560 + player.cameraOffset.Y) / 32); o++)
			{
				if (tilePointers[layer][o][i] != NULL)
				{
					tilePointers[layer][o][i]->Draw();
				}
			}
		}
	}
	else {
		int tileSize = tiles.size();
		for (int o = 0; o < tileSize; o++)
		{
			if (tiles[o].Layer == layer) tiles[o].Draw();
		}
		int entSize = entities.size();
		for (int o = 0; o < entSize; o++)
		{
			if (entities[o].Layer == layer) entities[o].Draw();
		}
	}

}
