#pragma once
class rRenderer
{
public:
	rRenderer();
	~rRenderer();

	bool fullScreen = false;
	bool isDisplayActive = false;

	int displayWidth = 960;
	int displayHeight = 540;

	void SetDisplayRes(int w, int h);
	ALLEGRO_DISPLAY * GetDisplayPtr();

	int Start();
	void VideoRestart();
	void EndFrame();

	void RefreshWindowTitle();

private:
	ALLEGRO_DISPLAY * display;
	ALLEGRO_BITMAP * displayBitmap;
	ALLEGRO_BITMAP * renderBitmap;
};

