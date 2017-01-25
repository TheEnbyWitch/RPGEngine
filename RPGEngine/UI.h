#pragma once
class UI
{
public:
	UI();
	~UI();
	ALLEGRO_BITMAP* windowBG;

	void DrawWindow(int x, int y, int width, int height);
	void DrawColoredWindow(int x, int y, int width, int height, ALLEGRO_COLOR color);

	void DrawMenuOption(int index, int x, int y, int width, int height, const char* text);
	void DrawColoredMenuOption(int index, int x, int y, int width, int height, const char* text, ALLEGRO_COLOR color);

	void DrawWindowWithText(const char * txt, int x, int y, int width, int height, int align = ALLEGRO_ALIGN_LEFT);
	void DrawColoredWindowWithText(const char* txt, int x, int y, int width, int height, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_LEFT);
	void DrawFPS(double dt);
};

