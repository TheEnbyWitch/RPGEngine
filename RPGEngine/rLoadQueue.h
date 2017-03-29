#pragma once

typedef enum {
	ASSET_MAP,
	ASSET_MENU,
	ASSET_TEXTURE
} rAssetType;

typedef struct rLoadQueueEntry_s {
	char name[1024];
	rAssetType type;
} rLoadQueueEntry_t;

class rLoadQueue
{
public:
	rLoadQueue();
	~rLoadQueue();

	void AddToQueue(char * assetname, rAssetType type);
	void LoadQueuedAssets();

private:
	bool finalized = false;
	int assetCount = 0;
	int loadedAssets = 0;
	std::vector<rLoadQueueEntry_t> assets;
};