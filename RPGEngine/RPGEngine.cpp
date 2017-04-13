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
gameState_e gameState = GAME_STATE_ENGINE_INTRO;


//GAME MODULES
rUI gUI;
rScript gScript;
rBitmap gBitmap;
rWorld gWorld;
rData gData;

rDialogue testDialogue;

rPlayer player;
ALLEGRO_MENU *menu;
ALLEGRO_MENU *dbg_menu;

bool bInitialized = false;
bool bUseIntro = false;
bool bUseAudio = true;

int __width = 960;
int __height = 540;
bool isShiftPressed = false;
char loadtxt[4][1024];

long loadtime = 0;

void init(void)
{
	memset(loadtxt, 0, sizeof(loadtxt));
	rpge_printf(
		"== init() ==\n%s\n%s\n%s\n",
		"*********************",
		"* INITIALIZING RPGe *",
		"*********************"
	);
	loadtime = clock();
	rpge_printf("Initializing Allegro %s\n", ALLEGRO_VERSION_STR);

	
	if (!al_init())
		abort_game("Failed to initialize allegro");
	
	gConsole.RegisterDVar("menu_verbose", DVAR_BOOL, 1);
	gConsole.RegisterDVar("version", DVAR_STRING, va("RPGE_%s", CONFIG_STR));

	rpge_printf("Creating the event queue\n"); 
	aEventQueue = al_create_event_queue();
	if (!aEventQueue)
		abort_game("Failed to create event queue");

	rpge_printf("Initializing PhysicsFS\n");
	PHYSFS_init(NULL);
	PHYSFS_mount("_build", NULL, 0);
	PHYSFS_mount("_resources", NULL, 1);
	PHYSFS_mount("_audio", "sound/", 1);
	PHYSFS_mount("vox", "vox/", 1);
	if (!PHYSFS_mount("playerdata", "playerdata/", 0))
	{
		rpge_printf("playerdata not found, creating new...\n");
		/*
		ALLEGRO_FILE * sv = al_fopen("playerdata", "wb");
		if (sv == 0) abort_game("Couldn't create playerdata!\n");
		al_fwrite(sv, gData.rawData, 138); // Write ZIP header
		al_fclose(sv);
		*/
		al_make_directory("playerdata");
		rpge_printf("playerdata mount result: %d\n", PHYSFS_mount("playerdata", "playerdata/", 0));
	}
	rpge_printf("Preparing the PhysFS file interface for use with Allegro\n");
	al_set_physfs_file_interface();


	rpge_printf("Initializing keyboard\n");
	if (!al_install_keyboard())
		abort_game("Failed to install keyboard");

	rpge_printf("Initializing mouse\n");
	if (!al_install_mouse())
		abort_game("Failed to install mouse");

	rpge_printf("Initializing image addon\n");
	if (!al_init_image_addon())
		abort_game("Failed to initialize image addon");

	rpge_printf("Initializing audio addon\n");
	if (!al_install_audio())
		abort_game("Failed to initialize audio!");
	if (!al_init_acodec_addon())
		abort_game("Failed to initialize audio codecs!");

	rpge_printf("Initializing font addon\n");
	al_init_font_addon();
	rpge_printf("Initializing TTF addon\n");
	al_init_ttf_addon();

	font = al_load_ttf_font("lucon.ttf", 12, 0);
	if (!font)
		abort_game("Font not found");
	gameLogo = al_load_bitmap("gmLogo.bmp");
	gUI.windowBG = al_load_bitmap("window_bg.tga");

	PHYSFS_setWriteDir("playerdata/");

	rpge_printf("Reading save data\n");
	gData.Init("player");
	gSound.Init();

	rpge_printf("Creating display\n");
	
	if (!gRenderer.Start())
		abort_game("Failed to create display");


	gameLogo = al_load_bitmap("gmLogo.bmp");
	gUI.windowBG = al_load_bitmap("window_bg.tga");

	DrawLoadWindow("Creating timer...", 0, 17);
	rpge_printf("Creating timer\n");
	aTimer = al_create_timer(1.0 / 60);
	if (!aTimer)
		abort_game("Failed to create timer");

	/*
	rpge_printf("Display modes: \n");
	for (int i = 0; i < al_get_num_display_modes(); i++)
	{
		ALLEGRO_DISPLAY_MODE   disp_data;
		al_get_display_mode(i, &disp_data);
		rpge_printf("%dx%d %dHz\n", disp_data.width, disp_data.height, disp_data.refresh_rate);
	}*/


	DrawLoadWindow("Registering event sources...", 0, 19);
	rpge_printf("Registering event sources\n");
	al_register_event_source(aEventQueue, al_get_keyboard_event_source());
	al_register_event_source(aEventQueue, al_get_timer_event_source(aTimer));

	gGameInfo.Init();

	menu = al_build_menu(gGameInfo.GetMenuInfo());
	if (IsDebug)
	{
		//al_set_display_menu(gRenderer.GetDisplayPtr(), menu);

		al_register_event_source(aEventQueue, al_get_default_menu_event_source());
	}
	DrawLoadWindow("Loading game assets...", -1, 20);
	gScript.ExecuteScript();
	gLoadQueue.LoadQueuedAssets();

	DrawLoadWindow("Preparing the player entity...", -1, 95);
	player.Activate("player", false);
	player.SetImage("Actor");
	player.useEmissive = true;

	initialize_assets();

	loadtime = clock() - loadtime;

	DrawLoadWindow(va("Game loaded in %.4f ms", (loadtime/1000.0f)), -1, 100);
	al_rest(1);
	testDialogue.SetContent("This is a test dialogue! I might be working!");
	testDialogue.SetSpeaker("A person");
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

bool showDbgMenu = false;
bool showDbgInfo = IsDebug;

bool showFPS = true;
bool showTOD = true;
bool showPlayerPos = true;

int frames = 0;
int gf = 0;
std::string drawBitmap;
double prevTimestamp = 0;
double curTimestamp = 0;
uint32_t lineOffset = 0;
int menuIndex = 0;
double deltaTime = 0.0;
float menuIndexSelectFrac[maxMenuIndex];

float * stuff = 0;

void EntFrameCallback(rEntity * self)
{
	self->Move(1, 0);
}

void game_loop(void)
{
	bool redraw = true;
	al_start_timer(aTimer);
	//gSound.PlayMusic("ilive.flac");

	while (1) {
		ALLEGRO_EVENT event;
		al_wait_for_event(aEventQueue, &event);

		if (event.type == ALLEGRO_EVENT_MENU_CLICK) {
#define MN_ACTN_T(id, var) if(event.user.data1 == id) var = !var;
#define MN_ACTN(id, action) if(event.user.data1 == id) action
			
			MN_ACTN_T(3, showFPS);
			MN_ACTN_T(4, showTOD);
			MN_ACTN_T(5, showPlayerPos);

			MN_ACTN(6, exit(1));
			MN_ACTN(gGameInfo.aboutID, al_show_native_message_box(al_get_current_display(), "About", ENGINE_STR, "Made by Ray1235", NULL, NULL));
		}
		
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			if (bInitialized)
			{
				frames++;
				gSound.Frame();
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
				else if (gameState == GAME_STATE_DIALOGUE)
				{
					testDialogue.Frame();
				}
				else if (gameState == GAME_STATE_INGAME)
				{
					/*if (0)//(frames % 60 == 0)
					{
						rEntity *t = rEntity::SpawnEntity();
						t->Activate();
						t->SetImage("Actor");
						t->useEmissive = true;
						t->SetFrameCallback(EntFrameCallback);
					}*/
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
			if (event.keyboard.keycode == ALLEGRO_KEY_TILDE)
			{
				showCon = !showCon;
				//consoleInput.clear();
			}
			if (!showCon)
			{

				if (event.keyboard.keycode == ALLEGRO_KEY_P)
				{
					//stuff = gSound.PlayVoiceover("vox/mop.wav");
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_HOME && IsDebug)
				{
					showDbgMenu = !showDbgMenu;
					if (showDbgMenu) al_set_display_menu(gRenderer.GetDisplayPtr(), menu);
					else al_set_display_menu(gRenderer.GetDisplayPtr(), NULL);
				}
				for (int i = 0; i < Menus.size(); i++)
				{
					rMenu * menu = &Menus[i];
					if (menu->isActive && gameState == GAME_STATE_MENU)
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
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					//gSound.PlayMusic("ilive.flac");
					gameState = GAME_STATE_MENU;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_Z)
				{
					testDialogue.active = true;
					gameState = GAME_STATE_DIALOGUE;
				}
				if (gameState == GAME_STATE_DIALOGUE)
				{
					testDialogue.Key(event.keyboard.keycode);
				}
				if ((event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_SPACE) && gameState == GAME_STATE_INGAME)
				{
					rVector2 dir = rEntity::GetVectorForDirection(player.Direction);
					for (auto ent : entityList)
					{
						if (player.PositionX + (dir.X * 32) == ent->PositionX && player.PositionY + (dir.Y * 32) == ent->PositionY)
						{
							ent->Interact();
						}
					}
				}
			}
			else {
				if (event.keyboard.keycode == ALLEGRO_KEY_PGUP)
				{
					lineOffset++;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_PGDN)
				{
					if (lineOffset > 0) lineOffset--;
				}
				gConsole.Key(event.keyboard.keycode);
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
			if (gameState == GAME_STATE_DIALOGUE)
			{
				gWorld.Draw();
				testDialogue.Draw();
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

			
			deltaTime = curTimestamp - prevTimestamp;
			if(showDbgInfo) gUI.DrawDebugInfo(deltaTime);
			prevTimestamp = curTimestamp;
			if (stuff != 0) gUI.DrawColoredWindowWithText(va("%f", *stuff), 50, 10, 60, 15, al_map_rgb(255,255,255));
			gRenderer.EndFrame();
		}
	}
}

int loadprog = 0;

void DrawLoadWindow(const char * text, int index, int prog)
{
	if (prog != -1) loadprog = prog;
	if (index == -1)
	{
		memset(loadtxt, 0, sizeof(loadtxt));
		index = 0;
	}
	strcpy(loadtxt[index], text);
	//rpge_printf(va("%s\n", loadtxt[index]));
	al_clear_to_color(al_map_rgb(0, 0, 0));
	std::string resultConLog;
	int llo = 0;
	int currentLines = 0;
	int lines = 0;
	for (int i = 0; i < consoleLog.length(); i++)
	{
		if (consoleLog[i] == '\n')
			lines++;
	}
	if (llo + MAX_LINES_SHOWN > lines) llo = lines - MAX_LINES_SHOWN;
	for (int i = consoleLog.length() - 2; i >= 0; i--)
	{
		if (consoleLog[i] == '\n')
		{
			currentLines++;
		}
		if (currentLines > MAX_LINES_SHOWN + llo)
			break;
		if (currentLines < llo) continue;
		resultConLog.insert(0, 1, consoleLog[i]);
	}
	al_draw_multiline_text(font, al_map_rgb(255, 255, 255), __width / 2, 160, __width, 13, ALLEGRO_ALIGN_CENTER, va("%s\n%s\n%s\n%s", loadtxt[0], loadtxt[1], loadtxt[2], loadtxt[3]));
	gUI.DrawColoredWindow(6, __height - (13 + 6), __width - (6 * 2), 14, al_map_rgb(0, 128, 255));
	gUI.DrawColoredWindowWithText(va("%d%%", loadprog), 6, __height - (13 + 6), (__width - (6 * 2)) * (loadprog / 100.0f), 14, al_map_rgb(0, 255, 0), ALLEGRO_ALIGN_RIGHT);
	gUI.DrawColoredWindowWithText(resultConLog.c_str(), 6, __height - (((MAX_LINES_SHOWN + 1) * 13) + 6 + 14 + 6), __width - (6 * 2), (MAX_LINES_SHOWN + 1) * 13, al_map_rgb(0, 128, 255));
	gRenderer.EndFrame();
	//rpge_printf("[RPGE] DrawLoadWindow called\n");
}

int game_main(int argc, char* argv[])
{
	init();
	game_loop();
	shutdown();
	return 0;
}

// Parappa The Wrappa
int main(int argc, char* argv[])
{
	game_main(argc, argv);
}