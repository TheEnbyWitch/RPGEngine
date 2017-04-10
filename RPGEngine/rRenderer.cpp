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
	al_register_event_source(aEventQueue, al_get_display_event_source(display));

	isDisplayActive = true;
	return (int)display;
}

void rRenderer::VideoRestart()
{
	rpge_printf("[rRenderer] VideoRestart doesn't work properly yet\n");
	//return;

	if (!isDisplayActive)
	{
		rpge_printf("[rRenderer] Display is not active, cannot restart video\n");
		return;
	}
	al_set_display_menu(gRenderer.GetDisplayPtr(), NULL);
	al_destroy_user_event_source(al_get_display_event_source(display));
	al_set_target_bitmap(displayBitmap);
	al_destroy_display(display);
	//al_destroy_bitmap(displayBitmap);
	al_destroy_bitmap(renderBitmap);
	Start();
	if (IsDebug)
	{
		if (showDbgMenu) al_set_display_menu(gRenderer.GetDisplayPtr(), menu);
		else al_set_display_menu(gRenderer.GetDisplayPtr(), NULL);
	}
}

void rRenderer::EndFrame()
{
	al_set_target_bitmap(displayBitmap);
	al_draw_scaled_bitmap(renderBitmap, 0, 0, __width, __height, 0, 0, displayWidth, displayHeight, ALLEGRO_MIN_LINEAR);
	if (showCon)
	{
		std::string resultConLog;
		int currentLines = 0;
		int lines = 0;
		for (int i = 0; i < consoleLog.length(); i++)
		{
			if (consoleLog[i] == '\n')
				lines++;
		}
		if (lineOffset + MAX_LINES_SHOWN > lines) lineOffset = lines - MAX_LINES_SHOWN;
		for (int i = consoleLog.length() - 2; i >= 0; i--)
		{
			if (consoleLog[i] == '\n')
			{
				currentLines++;
			}
			if (currentLines > MAX_LINES_SHOWN + lineOffset)
				break;
			if (currentLines < lineOffset) continue;
			resultConLog.insert(0, 1, consoleLog[i]);
		}

		gUI.DrawColoredWindowWithText(resultConLog.c_str(), 6, displayHeight - (((MAX_LINES_SHOWN + 1) * 13) + 6 + 14 + 6), displayWidth - (6 * 2), (MAX_LINES_SHOWN + 1) * 13, al_map_rgb(0, 128, 255));
		gUI.DrawColoredWindowWithText(va("%s%s%s", ENGINE_STR, consoleInput.c_str(), (frames % 40 > 20 ? "_" : "")), 6, displayHeight - (13 + 6), displayWidth - (6 * 2), 14, al_map_rgb(255, 255, 0), ALLEGRO_ALIGN_LEFT);
	}
	al_set_target_bitmap(renderBitmap);
	al_flip_display();
	RefreshWindowTitle();
}

void rRenderer::RefreshWindowTitle()
{
	if (!bInitialized)
	{
#ifdef _WIN32
		SetConsoleTitle(TEXT(va("[RPGEngineConsole] LOADING (%s)", (IsDebug ? "dev" : "ship"))));
#endif
		al_set_window_title(gRenderer.GetDisplayPtr(), va("[RPGEngine] LOADING (%s)", (IsDebug ? "dev" : "ship")));
	}
	else
	{
#ifdef _WIN32
		SetConsoleTitle(TEXT(va("[RPGEngineConsole] %s (%s)", gGameInfo.gameName, (IsDebug ? "dev" : "ship"))));
#endif
		al_set_window_title(gRenderer.GetDisplayPtr(), va("[RPGEngine] %s (%s) | Map: %s | Menu: %s | Current music playing: %s", gGameInfo.gameName, (IsDebug ? "dev" : "ship"), gWorld.currentLevel, activeMenu, currentMus));
	}
}
