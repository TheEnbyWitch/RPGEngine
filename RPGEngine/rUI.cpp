#include "stdafx.h"
#include "rUI.h"


rUI::rUI()
{
}


rUI::~rUI()
{
}

void rUI::DrawWindow(int x, int y, int width, int height)
{
	al_draw_bitmap_region(windowBG, 0, 0, 4, 4, x-4, y-4, NULL);
	al_draw_bitmap_region(windowBG, 12, 12, 4, 4, x + width, y + height, NULL);
	al_draw_scaled_bitmap(windowBG, 4, 4, 4, 4, x, y, width, height, NULL);
}

void rUI::DrawColoredWindow(int x, int y, int width, int height, ALLEGRO_COLOR color)
{
	al_draw_tinted_bitmap_region(windowBG, color, 0, 0, 4, 4, x - 4, y - 4, NULL);
	al_draw_tinted_bitmap_region(windowBG, color, 12, 12, 4, 4, x + width, y + height, NULL);
	al_draw_tinted_scaled_bitmap(windowBG, color, 4, 4, 4, 4, x, y, width, height, NULL);
}

void rUI::DrawMenuOption(int index, int x, int y, int width, int height, const char * text, rMenu * parent)
{
	DrawColoredMenuOption(index, x, y, width, height, text, al_map_rgb(0, 128, 255), parent);
}

void rUI::DrawColoredMenuOption(int index, int x, int y, int width, int height, const char * text, ALLEGRO_COLOR color, rMenu * parent)
{
	int rx = x;
	int ry = y;
	int rw = width;
	int rh = height;
	ALLEGRO_COLOR rc = color;
	if (parent != NULL)
	{
		rx += (int)(8.0f * parent->vars.menuIndexSelectFrac[index]);
		//rw -= 8 * menuIndexSelectFrac[index];
		rc.r += 60.0f * parent->vars.menuIndexSelectFrac[index] / 255.0f;
		rc.g += 60.0f * parent->vars.menuIndexSelectFrac[index] / 255.0f;
		rc.b += 60.0f * parent->vars.menuIndexSelectFrac[index] / 255.0f;
		rc.r = __min(rc.r, 1);
		rc.g = __min(rc.g, 1);
		rc.b = __min(rc.b, 1);
	}
	else
	{
		rx += (int)(8.0f * menuIndexSelectFrac[index]);
		//rw -= 8 * menuIndexSelectFrac[index];
		rc.r += 60.0f * menuIndexSelectFrac[index] / 255.0f;
		rc.g += 60.0f * menuIndexSelectFrac[index] / 255.0f;
		rc.b += 60.0f * menuIndexSelectFrac[index] / 255.0f;
		rc.r = __min(rc.r, 1);
		rc.g = __min(rc.g, 1);
		rc.b = __min(rc.b, 1);
	}
	DrawColoredWindowWithText(text, rx, ry, rw, rh, rc, ALLEGRO_ALIGN_CENTER);
}

void rUI::DrawColoredText(const char * txt, int x, int y, int width, ALLEGRO_COLOR color, int align)
{
	if (align == ALLEGRO_ALIGN_LEFT)
		al_draw_multiline_text(font, color, x + 2, y + 2, width - 4, 13, align, txt);
	else if (align == ALLEGRO_ALIGN_CENTER)
		al_draw_multiline_text(font, color, x + ((width - 4) / 2) + 2, y + 2, width - 4, 13, align, txt);
	else
		al_draw_multiline_text(font, color, x + width - 2, y + 2, width - 4, 13, align, txt);
}

void rUI::DrawWindowWithText(const char * txt, int x, int y, int width, int height, int align)
{
	DrawColoredWindowWithText(txt, x, y, width, height, al_map_rgb(255, 255, 255), align);
}

void rUI::DrawColoredWindowWithText(const char * txt, int x, int y, int width, int height, ALLEGRO_COLOR color, int align)
{
	DrawColoredWindowWithColoredText(txt, x, y, width, height, color, al_map_rgb(255,255,255), align);
}

void rUI::DrawColoredWindowWithColoredText(const char * txt, int x, int y, int width, int height, ALLEGRO_COLOR color, ALLEGRO_COLOR textclr, int align)
{
	DrawColoredWindow(x, y, width, height, color);
	if (align == ALLEGRO_ALIGN_LEFT)
		al_draw_multiline_text(font, textclr, x + 2, y + 2, width - 4, 13, align, txt);
	else if (align == ALLEGRO_ALIGN_CENTER)
		al_draw_multiline_text(font, textclr, x + ((width - 4) / 2) + 2, y + 2, width - 4, 13, align, txt);
	else
		al_draw_multiline_text(font, textclr, x + width - 2, y + 2, width - 4, 13, align, txt);
}

int fpsval[60];

void updatefps(int t)
{
	for (int i = 1; i < 60; i++)
	{
		fpsval[i - 1] = fpsval[i];
	}
	fpsval[59] = t;
}

int getavgfps()
{
	double a = 0;
	for (int i = 0; i < 60; i++)
	{
		a += fpsval[i];
	}
	a = a / 60.0f;
	return (int)a;
}

void rUI::DrawDebugInfo(double dt)
{
	//fps
	updatefps((int)(1 / dt));
	char f[256];
	strcpy(f, va("%d FPS", getavgfps()));
	if (getavgfps() > 30)
	{
		if (showFPS) DrawColoredWindowWithText(f, __width - (12+al_get_text_width(font, f)), 6, al_get_text_width(font, f)+6, 14, al_map_rgb(0, 255, 0), ALLEGRO_ALIGN_RIGHT);
	}
	else
	{
		if (showFPS) DrawColoredWindowWithText(f, __width - (12 + al_get_text_width(font, f)), 6, al_get_text_width(font, f) + 6, 14, al_map_rgb(255, 0, 0), ALLEGRO_ALIGN_RIGHT);
	}
	// time of day
	char tod[256];
	strcpy(tod, va("TOD: %d:%02d", (int)(gWorld.timeOfDay * 24), (int)(gWorld.timeOfDay * 1440) % 60));
	if (showTOD) DrawColoredWindowWithText(tod, __width - (12 + al_get_text_width(font, tod)), 26, al_get_text_width(font, tod) + 6, 14, gWorld.GetColorTint(), ALLEGRO_ALIGN_RIGHT);
	// player pos
	char plrpos[256];
	strcpy(plrpos, va("Player Pos X: %d | Y: %d", player.PositionX/32, player.PositionY/32));
	if (showPlayerPos) DrawColoredWindowWithText(plrpos, __width - (12 + al_get_text_width(font, plrpos)), 46, al_get_text_width(font, plrpos) + 6, 14, al_map_rgb(128,255,0), ALLEGRO_ALIGN_RIGHT);
	// TODO
}

