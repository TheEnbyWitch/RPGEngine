#pragma once
class rTile : public rEntity
{
public:
	rTile();
	rTile(int x, int y, int layer, char * name);
	~rTile();

	int tileID;
	int columns;

	bool isRegionCached = false;
	rScaledRegion_t cachedRegion;

	rScaledRegion_t GetScaledRegionForTile();

	void Draw();
	void Cache();
};

