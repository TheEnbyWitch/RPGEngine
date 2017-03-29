#include "stdafx.h"
#include "rLoadQueue.h"

rLoadQueue gLoadQueue;

rLoadQueue::rLoadQueue()
{
}


rLoadQueue::~rLoadQueue()
{
}

void rLoadQueue::AddToQueue(char * assetname, rAssetType type)
{
	if (finalized) return;
	rLoadQueueEntry_t e;
	strcpy(e.name, assetname);
	e.type = type;
	assets.push_back(e);
}

void rLoadQueue::LoadQueuedAssets()
{
	if (!finalized)
	{
		assetCount = assets.size();
		std::reverse(assets.begin(), assets.end());
	}
	while (assets.size() > 0)
	{
		rLoadQueueEntry_t * e = &assets[assets.size() - 1];
		DrawLoadWindow(va("Loading %s...", e->name), -1, 20 + (loadedAssets / (float)assetCount) * 75);
		switch (e->type)
		{
		case ASSET_MAP:
			gWorld.LoadMap(e->name);
			break;
		case ASSET_MENU:
			Menus.push_back(rMenu::ReadMenu(e->name));
			break;
		case ASSET_TEXTURE:
			gBitmap.GetBitmap(e->name, false, true);
			break;
		default:
			break;
		}
		loadedAssets++;
		assets.pop_back();
	}
}
