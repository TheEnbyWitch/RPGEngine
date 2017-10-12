#ifndef __RPG_STDAFX
#define __RPG_STDAFX

#ifdef _MSC_VER
#pragma warning( disable : 4018)
#endif

#ifdef _UNICODE
#undef _UNICODE // if it was defined, undefine, it breaks stuff
#endif

#define _CRT_SECURE_NO_WARNINGS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) // macro for getting only the filename
#define NOMINMAX

#define maxMenuIndex 8		// max menu choices

#define MAX_LINES_SHOWN 20		// max lines to show in console
//#define BITMAP_VERBOSE			// game will spit out a critical error if a bitmap is not found (only for GetBitmap())
#ifdef _DEBUG
#define CONFIG_STR				"DEV"
#define IsDebug					true
#else
#define CONFIG_STR				"SHIP"
#define IsDebug					false
#endif
#define ENGINE_STR				"0.0.0: RPGE_"CONFIG_STR"> "

#ifdef _WIN32
#include "targetver.h"
#endif
#include "Branding.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <physfs.h>
#include "tmx\NLTmxMap.h"
#include "json.hpp"

#include "AngelScript\include\angelscript.h"
#include "AngelScript\add_on\scriptarray\scriptarray.h"
#include "AngelScript\add_on\scriptbuilder\scriptbuilder.h"
#include "AngelScript\add_on\scriptstdstring\scriptstdstring.h"
#include "AngelScript\add_on\scriptmath\scriptmath.h"
#include "AngelScript\add_on\scripthandle\scripthandle.h"
#include "AngelScript\add_on\weakref\weakref.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_physfs.h>
#include <allegro5\keyboard.h>

#include <allegro5\allegro_native_dialog.h>

extern ALLEGRO_EVENT_QUEUE* aEventQueue;
extern ALLEGRO_TIMER* aTimer;
extern ALLEGRO_DISPLAY* aDisplay;
extern ALLEGRO_FONT *font;

extern std::ifstream gameConfig;

extern std::string consoleLog;
extern std::string consoleInput;

extern bool bInitialized;
extern int menuIndex;
extern float menuIndexSelectFrac[maxMenuIndex];

extern int __width;
extern int __height;

extern double deltaTime;

extern bool showCon;
extern bool isShiftPressed;

extern bool showDbgMenu;
extern bool showFPS;
extern bool showTOD;
extern bool showPlayerPos;

extern uint32_t lineOffset;
extern int frames;

typedef enum {
	GAME_STATE_ENGINE_INTRO,
	GAME_STATE_INTRO,
	GAME_STATE_MENU,
	GAME_STATE_INGAME,
	GAME_STATE_DIALOGUE
} gameState_e;

extern gameState_e gameState;

#include "Common.h"
#include "rDVar.h"
#include "rCommand.h"
#include "rConsole.h"
#include "rGameInfo.h"
#include "rRenderer.h"
#include "rData.h"
#include "rSound.h"
#include "rLoadQueue.h"
#include "rMath.h"
#include "rUI.h"
#include "rEntity.h"
#include "rEntityScriptWrapper.h"
#include "rCollision.h"
#include "rTile.h"
#include "rMap.h"
#include "rMenu.h"
#include "rWorld.h"
#include "rDialogue.h"
#include "rPlayer.h"
#include "rScriptEngineHelper.h"
#include "rScript.h"
#include "rBitmap.h"

extern rGameInfo gGameInfo;
extern rRenderer gRenderer;
extern rConsole gConsole;
extern rUI gUI;
extern rSound gSound;
extern rScript gScript;
extern rBitmap gBitmap;
extern rWorld gWorld;
extern rData gData;
extern rLoadQueue gLoadQueue;
extern rDialogue gDialogue;

extern rPlayer player;

extern ALLEGRO_MENU *menu;
extern ALLEGRO_MENU *dbg_menu;
extern char currentMus[256];

void initialize_assets();
void DrawLoadWindow(const char * text, int index = 0, int prog = -1);

#endif