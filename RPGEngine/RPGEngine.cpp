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
rUI gUI;

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

	initialize_menus();

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

MENU_ACTION_FUNC(StartGame)
{
	rpge_printf("game should start\n");
}

MENU_ACTION_FUNC(QuitGame)
{
	exit(0);
}

void initialize_menus()
{
#define BUTTON_RECT(index) rRect_t{ (640/2)-200, 16+(index * 24), 400, 16}
	rMenu main;
	main.name = "main";
	main.isActive = false;

	rMenuItem_t start;
	start.text = "START";
	start.type = ITEM_TYPE_BUTTON;
	start.buttonAttributes.index = 0;
	start.rect = BUTTON_RECT(3);
	start.buttonAttributes.onClickFunc = &StartGame;
	main.items.push_back(start);

	rMenuItem_t options;
	options.text = "OPTIONS";
	options.type = ITEM_TYPE_BUTTON;
	options.buttonAttributes.index = 1;
	options.rect = BUTTON_RECT(4);
	main.items.push_back(options);

	rMenuItem_t quit;
	quit.text = "QUIT";
	quit.type = ITEM_TYPE_BUTTON;
	quit.buttonAttributes.index = 2;
	quit.rect = BUTTON_RECT(5);
	quit.color = al_map_rgb(255, 0, 0);
	quit.buttonAttributes.onClickFunc = &QuitGame;
	main.items.push_back(quit);

	rMenuItem_t title;
	title.text = "something something the game";
	title.rect = BUTTON_RECT(0);
	title.type = ITEM_TYPE_TEXT;
	title.textAttributes.align = ALLEGRO_ALIGN_CENTER;
	main.items.push_back(title);

	Menus.push_back(main);
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
				if (!strcmp(menu->name, activeMenu))
				{
					menu->isActive = true;
					menu->Key(event.keyboard.keycode);
				}
				else
				{
					menu->isActive = false;
				}
			}
			if (!showCon)
			{
				/*
				if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					menuIndex++;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					menuIndex--;
				}
				*/
			}
			else {
				// NEEDS A REWRITE DAMN IT
				char input = (char)event.keyboard.keycode+ 'a' - 1 + (event.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT ? 'A'-'a' : 0 );
				if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
					if(consoleInput.length() > 0) consoleInput.erase(consoleInput.length() - 1);
				if(input >= 32)
					consoleInput += input;
			}
		}

		if (redraw && al_is_event_queue_empty(aEventQueue)) {
			/*
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
				menuIndexSelectFrac[i] = __min(__max(0.0f, menuIndexSelectFrac[i]), 1.0f);
			}*/
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
				for (int i = 0; i < Menus.size(); i++)
				{
					rMenu * menu = &Menus[i];
					if (!strcmp(menu->name, activeMenu))
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
				
				gUI.DrawColoredWindowWithText(resultConLog.c_str(), 6, 480 - (((MAX_LINES_SHOWN+1) * 13) + 6 + 14 + 6), 628, (MAX_LINES_SHOWN + 1) *13, al_map_rgb(0, 128, 255));
				gUI.DrawColoredWindowWithText(consoleInput.c_str(), 6, 480 - (13 + 6), 628, 14, al_map_rgb(255, 255, 0), ALLEGRO_ALIGN_LEFT);
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