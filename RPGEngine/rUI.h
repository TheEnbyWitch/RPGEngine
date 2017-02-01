#pragma once
#include "stdafx.h"
#include "rMenu.h"
class rUI
{
public:
	rUI();
	~rUI();
	ALLEGRO_BITMAP* windowBG;

	void DrawWindow(int x, int y, int width, int height);
	void DrawColoredWindow(int x, int y, int width, int height, ALLEGRO_COLOR color);

	void DrawMenuOption(int index, int x, int y, int width, int height, const char* text, rMenu * parent = NULL);
	void DrawColoredMenuOption(int index, int x, int y, int width, int height, const char * text, ALLEGRO_COLOR color, rMenu * parent = NULL);

	void DrawColoredText(const char* txt, int x, int y, int width, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_LEFT);
	void DrawWindowWithText(const char * txt, int x, int y, int width, int height, int align = ALLEGRO_ALIGN_LEFT);
	void DrawColoredWindowWithText(const char* txt, int x, int y, int width, int height, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_LEFT);
	void DrawColoredWindowWithColoredText(const char * txt, int x, int y, int width, int height, ALLEGRO_COLOR color, ALLEGRO_COLOR textclr, int align);
	void DrawFPS(double dt);
};

