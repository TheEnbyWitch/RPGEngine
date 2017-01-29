#pragma once
class rTile : public rEntity
{
public:
	rTile();
	rTile(int x, int y, char * name);
	~rTile();

	void Draw();
};

