#include "stdafx.h"
#include "rRenderer.h"

rRenderer gRenderer;

rRenderer::rRenderer()
{
}


rRenderer::~rRenderer()
{
}

void rRenderer::SetDisplayRes(int w, int h)
{
	displayWidth = w;
	displayHeight = h;
}

ALLEGRO_DISPLAY * rRenderer::GetDisplayPtr()
{
	return display;
}

int rRenderer::Start()
{
	al_set_new_display_flags(fullScreen ? ALLEGRO_FULLSCREEN_WINDOW : ALLEGRO_WINDOWED);
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	display = al_create_display(displayWidth, displayHeight);
	displayBitmap = al_get_target_bitmap();
	renderBitmap = al_create_bitmap(__width, __height);
	al_set_target_bitmap(renderBitmap);
	isDisplayActive = true;
	return (int)display;
}

void rRenderer::VideoRestart()
{
	if (!isDisplayActive)
	{
		rpge_printf("[rRenderer] Display is not active, cannot restart video");
		return;
	}
	al_set_target_bitmap(displayBitmap);
	al_destroy_display(display);
	//al_destroy_bitmap(displayBitmap);
	al_destroy_bitmap(renderBitmap);
	Start();
}

void rRenderer::EndFrame()
{
	al_set_target_bitmap(displayBitmap);
	al_draw_scaled_bitmap(renderBitmap, 0, 0, __width, __height, 0, 0, displayWidth, displayHeight, ALLEGRO_MIN_LINEAR);
	al_set_target_bitmap(renderBitmap);
	al_flip_display();
}
