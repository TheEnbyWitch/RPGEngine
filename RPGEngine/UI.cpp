#include "stdafx.h"
#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::DrawWindow(int x, int y, int width, int height)
{
	al_draw_bitmap_region(windowBG, 0, 0, 4, 4, x-4, y-4, NULL);
	al_draw_bitmap_region(windowBG, 12, 12, 4, 4, x + width, y + height, NULL);
	al_draw_scaled_bitmap(windowBG, 4, 4, 4, 4, x, y, width, height, NULL);
}

void UI::DrawColoredWindow(int x, int y, int width, int height, ALLEGRO_COLOR color)
{
	al_draw_tinted_bitmap_region(windowBG, color, 0, 0, 4, 4, x - 4, y - 4, NULL);
	al_draw_tinted_bitmap_region(windowBG, color, 12, 12, 4, 4, x + width, y + height, NULL);
	al_draw_tinted_scaled_bitmap(windowBG, color, 4, 4, 4, 4, x, y, width, height, NULL);
}

void UI::DrawWindowWithText(const char * txt, int x, int y, int width, int height, int align)
{
	DrawColoredWindowWithText(txt, x, y, width, height, al_map_rgb(255, 255, 255), align);
}

void UI::DrawColoredWindowWithText(const char * txt, int x, int y, int width, int height, ALLEGRO_COLOR color, int align)
{
	DrawColoredWindow(x, y, width, height, color);
	if(align == ALLEGRO_ALIGN_LEFT)
		al_draw_multiline_text(font, al_map_rgb(255, 255, 255), x + 2, y + 2, width - 4, 13, align, txt);
	else if(align == ALLEGRO_ALIGN_CENTER)
		al_draw_multiline_text(font, al_map_rgb(255, 255, 255), x + ((width - 4)/2) + 2, y + 2, width - 4, 13, align, txt);
	else
		al_draw_multiline_text(font, al_map_rgb(255, 255, 255), x + width - 2, y + 2, width - 4, 13, align, txt);
}

int fpsval[10];

void updatefps(int t)
{
	for (int i = 1; i < 10; i++)
	{
		fpsval[i - 1] = fpsval[i];
	}
	fpsval[9] = t;
}

int getavgfps()
{
	double a = 0;
	for (int i = 0; i < 10; i++)
	{
		a += fpsval[i];
	}
	a = a / 10.0f;
	return (int)a;
}

void UI::DrawFPS(double dt)
{
	updatefps((int)(1 / dt));
	char * f = va("%d FPS", getavgfps());
	if (dt < (1.0f / 30.0f))
	{
		DrawColoredWindowWithText(f, 640 - (12+al_get_text_width(font, f)), 6, al_get_text_width(font, f)+6, 14, al_map_rgb(0, 255, 0), ALLEGRO_ALIGN_RIGHT);
	}
	else
	{
		DrawColoredWindowWithText(f, 640 - (12 + al_get_text_width(font, f)), 6, al_get_text_width(font, f) + 6, 14, al_map_rgb(255, 0, 0), ALLEGRO_ALIGN_RIGHT);
	}
	delete f;
}

