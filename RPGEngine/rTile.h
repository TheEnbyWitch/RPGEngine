#ifndef __RPG_RTILE
#define __RPG_RTILE

class rTile
{
public:
	rTile();
	rTile(int x, int y, int layer, char * name);
	~rTile();

	int pX;
	int pY;

	char pathToImage[256];
	int Layer = 1;
	int Level = 1;
	int PositionX = 0;
	int PositionY = 0;

	int tileID;
	int columns;

	bool isRegionCached = false;
	rScaledRegion_t cachedRegion;
	ALLEGRO_BITMAP * cachedBitmap;

	rScaledRegion_t GetScaledRegionForTile();

	void Draw();
	void DrawToBitmap(int ofsX, int ofsY);
	void Cache();
};

#endif