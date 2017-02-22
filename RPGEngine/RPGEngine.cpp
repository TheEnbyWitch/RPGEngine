// RPGEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

ALLEGRO_EVENT_QUEUE* aEventQueue;
ALLEGRO_TIMER* aTimer;
ALLEGRO_DISPLAY* aDisplay;
ALLEGRO_FONT* font;
ALLEGRO_TEXTLOG *txtLog;

ALLEGRO_BITMAP* gameLogo;
ALLEGRO_BITMAP* actor;

std::ifstream gameConfig;
gameInfo_t gameInfo;
gameState_e gameState = GAME_STATE_ENGINE_INTRO;


//GAME MODULES
rUI gUI;
rScript gScript;
rBitmap gBitmap;
rWorld gWorld;

rPlayer player;

bool bInitialized = false;
bool bUseIntro = false;

int __width = 960;
int __height = 540;
bool isShiftPressed = false;

void init(void)
{
	rpge_printf(
		"== init() ==\n%s\n%s\n%s\n",
		"*********************",
		"* INITIALIZING RPGe *",
		"*********************"
	);

	rpge_printf("Initializing Allegro %s\n", ALLEGRO_VERSION_STR);

	if (!al_init())
		abort_game("Failed to initialize allegro");

	rpge_printf("Initializing PhysicsFS\n");
	PHYSFS_init(NULL);
	PHYSFS_mount("_build", NULL, 0);
	PHYSFS_mount("_resources", NULL, 1);
	PHYSFS_mount("_audio", "sound/", 1);


	rpge_printf("Initializing keyboard\n");
	if (!al_install_keyboard())
		abort_game("Failed to install keyboard");

	rpge_printf("Initializing mouse\n");
	if (!al_install_mouse())
		abort_game("Failed to install mouse");

	rpge_printf("Initializing image addon\n");
	if (!al_init_image_addon())
		abort_game("Failed to initialize image addon");

	rpge_printf("Preparing the PhysFS file interface for use with Allegro\n");
	al_set_physfs_file_interface();

	rpge_printf("Creating timer\n");
	aTimer = al_create_timer(1.0 / 60);
	if (!aTimer)
		abort_game("Failed to create timer");

	rpge_printf("Creating display\n");
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	//al_set_new_display_flags(ALLEGRO_OPENGL);
	aDisplay = al_create_display(__width, __height);
	/*
	rpge_printf("Display modes: \n");
	for (int i = 0; i < al_get_num_display_modes(); i++)
	{
		ALLEGRO_DISPLAY_MODE   disp_data;
		al_get_display_mode(i, &disp_data);
		rpge_printf("%dx%d %dHz\n", disp_data.width, disp_data.height, disp_data.refresh_rate);
	}*/
	if (!aDisplay)
		abort_game("Failed to create display");

	rpge_printf("Creating the event queue\n");
	aEventQueue = al_create_event_queue();
	if (!aEventQueue)
		abort_game("Failed to create event queue");

	rpge_printf("Initializing font addon\n");
	al_init_font_addon();
	rpge_printf("Initializing TTF addon\n");
	al_init_ttf_addon();

	font = al_load_ttf_font("lucon.ttf", 12, 0);
	if (!font)
		abort_game("Font not found");

	rpge_printf("Registering event sources\n");
	al_register_event_source(aEventQueue, al_get_keyboard_event_source());
	al_register_event_source(aEventQueue, al_get_timer_event_source(aTimer));
	al_register_event_source(aEventQueue, al_get_display_event_source(aDisplay));

	gScript.ExecuteScript();
	
	gameLogo = al_load_bitmap("gmLogo.bmp");
	gUI.windowBG = al_load_bitmap("window_bg.tga");

	player.Create();
	player.SetImage("Actor");

	initialize_assets();

	gameInfo.name = GAME_NAME;
	bInitialized = true;

#ifdef USE_INTRO
	std::ifstream gmIntro("gameintro");
	if (!gmIntro.good()) return;

	rpge_printf("%s\n", "Using game intro");
	gmIntro >> gameIntroJSON;
#endif
	rpge_printf("== init() finished ==\n");
}

void shutdown(void)
{
	if (aTimer)
		al_destroy_timer(aTimer);

	if (aDisplay)
		al_destroy_display(aDisplay);

	if (aEventQueue)
		al_destroy_event_queue(aEventQueue);

	PHYSFS_deinit();

	gameConfig.close();
}

void initialize_assets()
{
	
}

bool showCon = false;
int frames = 0;
int gf = 0;
std::string drawBitmap;
double prevTimestamp = 0;
double curTimestamp = 0;
uint32_t lineOffset = 0;
int menuIndex = 0;
double deltaTime = 0.0;
float menuIndexSelectFrac[maxMenuIndex];
void game_loop(void)
{
	bool redraw = true;
	al_start_timer(aTimer);

	if (!bInitialized)
	{
#ifdef _WIN32
		SetConsoleTitle(TEXT(va("[RPGEngineConsole] NO GAME (%s)", (_DEBUG ? "dev" : "ship"))));
#endif
		al_set_window_title(aDisplay, va("[RPGEngine] NO GAME (%s)", (_DEBUG ? "dev" : "ship")));
		abort_game("RPGEngine couldn't initialize game");
	} 
	else
	{
#ifdef _WIN32
		SetConsoleTitle(TEXT(va("[RPGEngineConsole] %s (%s)", gameInfo.name, (_DEBUG ? "dev" : "ship"))));
#endif
		al_set_window_title(aDisplay, va("[RPGEngine] %s (%s)",gameInfo.name, (_DEBUG ? "dev" : "ship")));
	}

	while (1) {
		ALLEGRO_EVENT event;
		al_wait_for_event(aEventQueue, &event);
		
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			if (bInitialized)
			{
				frames++;
				if (gameState == GAME_STATE_ENGINE_INTRO)
				{
					if (frames > 3 * 60)
					{
#ifdef USE_INTRO
						gameState = GAME_STATE_INTRO;
#else
						gameState = GAME_STATE_MENU;
#endif
						frames = 0;
					}
					
				}
				else if (gameState == GAME_STATE_INTRO)
				{
					
				}
				else if (gameState == GAME_STATE_MENU)
				{
					
				}
				else if (gameState == GAME_STATE_INGAME)
				{
					gWorld.Frame();
					ALLEGRO_KEYBOARD_STATE state;
					al_get_keyboard_state(&state);
					frames++;
					if (al_key_down(&state, ALLEGRO_KEY_DOWN))
					{
						player.Move(0, 1);
					}
					if (al_key_down(&state, ALLEGRO_KEY_UP))
					{
						player.Move(0, -1);
					}
					if (al_key_down(&state, ALLEGRO_KEY_LEFT))
					{
						player.Move(-1, 0);
					}
					if (al_key_down(&state, ALLEGRO_KEY_RIGHT))
					{
						player.Move(1, 0);
					}
					player.Frame();

				}
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT || event.keyboard.keycode == ALLEGRO_KEY_RSHIFT)
				isShiftPressed = false;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT || event.keyboard.keycode == ALLEGRO_KEY_RSHIFT)
				isShiftPressed = true; 
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_TILDE)
			{
				showCon = !showCon;
				consoleInput.clear();
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_PGUP)
			{
				lineOffset++;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_PGDN)
			{
				if (lineOffset > 0) lineOffset--;
			}
			for (int i = 0; i < Menus.size(); i++)
			{
				rMenu * menu = &Menus[i];
				if(menu->isActive)
					menu->Key(event.keyboard.keycode);
				if (strcmp(menu->name, activeMenu) == 0)
				{
					menu->isActive = true;
				}
				else
				{
					menu->isActive = false;
				}
			}
			if (!showCon)
			{
			}
			else {
				// NEEDS A REWRITE DAMN IT
				//char input = (char)event.keyboard.keycode+ 'a' - 1 + (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT ? 'A'-'a' : 0 );
				char input = GetCharFromKeycode(event.keyboard.keycode, (isShiftPressed ? ALLEGRO_KEYMOD_SHIFT : 0 ));
				if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
					if(consoleInput.length() > 0) consoleInput.erase(consoleInput.length() - 1);
				if(input > 0)
					consoleInput += input;
			}
		}

		if (redraw && al_is_event_queue_empty(aEventQueue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			curTimestamp = al_get_time();
			if (gameState == GAME_STATE_ENGINE_INTRO)
			{
				if(bInitialized)
					al_draw_multiline_text(font, al_map_rgb(255, 255, 255), __width / 2, __height / 2, __width, 13, ALLEGRO_ALIGN_CENTER, "POWERED BY\nRPGEngine");
				else
					al_draw_multiline_text(font, al_map_rgb(255, 255, 255), __width / 2, __height / 2, __width, 13, ALLEGRO_ALIGN_CENTER, "RPGEngine\nencountered an error! Check log!");
			}
			if (gameState == GAME_STATE_INTRO)
			{
				
			}
			if (gameState == GAME_STATE_INGAME)
			{
				gWorld.Draw();
			}
			if (gameState == GAME_STATE_MENU)
			{
				for (int i = 0; i < Menus.size(); i++)
				{
					rMenu * menu = &Menus[i];
					if (strcmp(menu->name, activeMenu) == 0)
					{
						menu->isActive = true;
						menu->Frame();
						menu->Draw();
					}
					else
					{
						menu->isActive = false;
					}
				}
			}

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
				if (lineOffset+MAX_LINES_SHOWN > lines) lineOffset = lines-MAX_LINES_SHOWN;
				for (int i = consoleLog.length() - 1; i >= 0; i--)
				{
					if (consoleLog[i] == '\n')
					{
						currentLines++;
					}
					if (currentLines > MAX_LINES_SHOWN+lineOffset)
						break;
					if (currentLines < lineOffset) continue;
					resultConLog.insert(0, 1, consoleLog[i]);	
				}
				
				gUI.DrawColoredWindowWithText(resultConLog.c_str(), 6, __height - (((MAX_LINES_SHOWN+1) * 13) + 6 + 14 + 6), __width - (6 * 2), (MAX_LINES_SHOWN + 1) *13, al_map_rgb(0, 128, 255));
				gUI.DrawColoredWindowWithText(va("%s%s%s", ENGINE_STR, consoleInput.c_str(), ( frames % 40 > 20 ? "_" : "")), 6, __height - (13 + 6), __width-(6*2), 14, al_map_rgb(255, 255, 0), ALLEGRO_ALIGN_LEFT);
			}
			deltaTime = curTimestamp - prevTimestamp;
			gUI.DrawDebugInfo(deltaTime);
			prevTimestamp = curTimestamp;
			al_flip_display();
		}
	}
}

int main(int argc, char* argv[])
{
	init();
	game_loop();
	shutdown();
}