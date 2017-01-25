// RPGEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

ALLEGRO_EVENT_QUEUE* aEventQueue;
ALLEGRO_TIMER* aTimer;
ALLEGRO_DISPLAY* aDisplay;
ALLEGRO_FONT* font;
ALLEGRO_TEXTLOG *txtLog;

ALLEGRO_BITMAP* gameLogo;


std::ifstream gameConfig;
gameInfo_t gameInfo;
gameState_e gameState = GAME_STATE_ENGINE_INTRO;

json gameIntroJSON;

//GAME MODULES
UI gUI;

bool bInitialized = false;
bool bUseIntro = false;

void init(void)
{
	//char * k = new char[500];
	//abort_game(k);
	if (!al_init())
		abort_game("Failed to initialize allegro");
	//al_open_native_text_log("Log", ALLEGRO_TEXTLOG_MONOSPACE | ALLEGRO_TEXTLOG_NO_CLOSE);
	rpge_printf(
		"%s\n%s\n%s\n",
		"*********************",
		"* INITIALIZING RPGe *",
		"*********************"
	);
	rpge_printf("Initializing PhysicsFS\n");
	PHYSFS_init(NULL);
	PHYSFS_mount("_build", NULL, 0);
	PHYSFS_mount("_resources", NULL, 1);
	PHYSFS_mount("_audio", "sound/", 1);

	if (!al_install_keyboard())
		abort_game("Failed to install keyboard");

	if (!al_install_mouse())
		abort_game("Failed to install mouse");

	if (!al_init_image_addon())
		abort_game("How come the image addon isn't loading dude");

	al_set_physfs_file_interface();

	aTimer = al_create_timer(1.0 / 60);
	if (!aTimer)
		abort_game("Failed to create timer");

	al_set_new_display_flags(ALLEGRO_WINDOWED);
	aDisplay = al_create_display(640, 480);
	rpge_printf("Display modes: \n");
	for (int i = 0; i < al_get_num_display_modes(); i++)
	{
		ALLEGRO_DISPLAY_MODE   disp_data;
		al_get_display_mode(i, &disp_data);
		rpge_printf("%dx%d %dHz\n", disp_data.width, disp_data.height, disp_data.refresh_rate);
	}
	if (!aDisplay)
		abort_game("Failed to create display");

	aEventQueue = al_create_event_queue();
	if (!aEventQueue)
		abort_game("Failed to create event queue");

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	font = al_load_ttf_font("lucon.ttf", 12, 0);
	if (!font)
		abort_game("Font not found");

	al_register_event_source(aEventQueue, al_get_keyboard_event_source());
	al_register_event_source(aEventQueue, al_get_timer_event_source(aTimer));
	al_register_event_source(aEventQueue, al_get_display_event_source(aDisplay));
	
	gameLogo = al_load_bitmap("gmLogo.bmp");
	gUI.windowBG = al_load_bitmap("window_bg.tga");

	gameInfo.name = GAME_NAME;
	bInitialized = true;

#ifdef USE_INTRO
	std::ifstream gmIntro("gameintro");
	if (!gmIntro.good()) return;

	rpge_printf("%s\n", "Using game intro");
	gmIntro >> gameIntroJSON;
#endif
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
bool showCon = false;
int frames = 0;
int gf = 0;
std::string drawBitmap;
double prevTimestamp = 0;
double curTimestamp = 0;
uint32_t lineOffset = 0;
int menuIndex = 0;
float menuIndexSelectFrac[maxMenuIndex];
void game_loop(void)
{
	bool redraw = true;
	al_start_timer(aTimer);

	if (!bInitialized)
	{
		al_set_window_title(aDisplay, "RPGEngine - NO GAME");
		rpge_printf("RPGEngine couldn't initialize game");
	} 
	else
	{
		al_set_window_title(aDisplay, gameInfo.name);
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
					frames++;
				}
				else if (gameState == GAME_STATE_INTRO)
				{
					
				}
				else if (gameState == GAME_STATE_MENU)
				{
					
				}
				else if (gameState == GAME_STATE_INGAME)
				{

				}
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {

			}
			if (event.keyboard.keycode == ALLEGRO_KEY_TILDE)
				showCon = !showCon;

			if (event.keyboard.keycode == ALLEGRO_KEY_PGUP)
			{
				lineOffset++;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_PGDN)
			{
				if (lineOffset > 0) lineOffset--;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				menuIndex++;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				menuIndex--;
			}
			//get_user_input();
		}

		if (redraw && al_is_event_queue_empty(aEventQueue)) {
			for (int i = 0; i < maxMenuIndex; i++)
			{
				if (menuIndex == i)
				{
					menuIndexSelectFrac[i] += 0.1f;
				}
				else
				{
					menuIndexSelectFrac[i] -= 0.1f;
				}
				if (menuIndexSelectFrac[i] > 1) menuIndexSelectFrac[i] = 1;
				if (menuIndexSelectFrac[i] < 0) menuIndexSelectFrac[i] = 0;
			}
			redraw = false;
			gf++;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			curTimestamp = al_get_time();
			if (gameState == GAME_STATE_ENGINE_INTRO)
			{
				if(bInitialized)
					al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 640 / 2, 480 / 2, 640, 13, ALLEGRO_ALIGN_CENTER, "POWERED BY\nRPGEngine");
				else
					al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 640 / 2, 480 / 2, 640, 13, ALLEGRO_ALIGN_CENTER, "RPGEngine\nencountered an error! Check log!");
			}
			if (gameState == GAME_STATE_INTRO)
			{
				
			}
			if (gameState == GAME_STATE_MENU)
			{
				gUI.DrawWindowWithText("GAME", 16, 16, 256, 16);
				gUI.DrawMenuOption(0, 16, 40, 256, 16, "START");
				gUI.DrawMenuOption(1, 16, 64, 256, 16, "OPTIONS");
				gUI.DrawColoredMenuOption(2, 16, 88, 256, 16, "QUIT", al_map_rgb(255, 0, 0));
			}

			if (showCon)
			{
				//std::string * conLogLines;
				int lines = 1;
				for (int i = 0; i < consoleLog.length(); i++)
					if (consoleLog[i] == '\n') lines++;
				//conLogLines = new std::string[lines];
				int pos = 0;
				for (int i = 0; i < lines; i++)
				{
					for (; pos < consoleLog.length() && consoleLog[pos] != '\n'; pos++)
					{
						//conLogLines[i].append(consoleLog[pos]);
					}
				}
				gUI.DrawColoredWindowWithText(consoleLog.c_str(), 6, 6, 628, 468, al_map_rgb(0, 128, 255));
				//delete conLogLines;
			}
			gUI.DrawFPS(curTimestamp - prevTimestamp);
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