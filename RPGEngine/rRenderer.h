#pragma once
class rRenderer
{
public:
	rRenderer();
	~rRenderer();

	bool fullScreen = true;
	bool isDisplayActive = false;

	int displayWidth = 1600;
	int displayHeight = 900;

	void SetDisplayRes(int w, int h);
	ALLEGRO_DISPLAY * GetDisplayPtr();

	int Start();
	void VideoRestart();
	void EndFrame();

private:
	ALLEGRO_DISPLAY * display;
	ALLEGRO_BITMAP * displayBitmap;
	ALLEGRO_BITMAP * renderBitmap;
};

