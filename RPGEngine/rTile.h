#pragma once
class rTile : public rEntity
{
public:
	rTile();
	rTile(int x, int y, int layer, char * name);
	~rTile();

	void Draw();
};

