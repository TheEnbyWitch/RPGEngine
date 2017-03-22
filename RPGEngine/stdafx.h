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

#define maxMenuIndex 128		// max menu choices

#define MAX_LINES_SHOWN 20		// max lines to show in console
//#define BITMAP_VERBOSE			// game will spit out a critical error if a bitmap is not found (only for GetBitmap())
#ifdef _DEBUG
#define CONFIG_STR				"DEV"
#else
#define CONFIG_STR				"SHIP"
#endif
#define ENGINE_STR				"0.0.0: RPGE_"CONFIG_STR"> "

#define __USE_ANGELSCRIPT		// uses angelscript

#ifdef _WIN32
#include "targetver.h"
#endif
#include "Branding.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <physfs.h>
#include "tmx\NLTmxMap.h"
#include "json.hpp"
#ifdef __USE_LUA
#include "Lua\lua.hpp"
#endif
#ifdef __USE_SQUIRREL
#include <squirrel.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#endif
#ifdef __USE_ANGELSCRIPT
#include "AngelScript\include\angelscript.h"
#include "AngelScript\add_on\scriptarray\scriptarray.h"
#include "AngelScript\add_on\scriptbuilder\scriptbuilder.h"
#include "AngelScript\add_on\scriptstdstring\scriptstdstring.h"
#include "AngelScript\add_on\scriptmath\scriptmath.h"
#include "AngelScript\add_on\scripthandle\scripthandle.h"
#include "AngelScript\add_on\weakref\weakref.h"
#endif

#include <allegro5\allegro.h>
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

typedef enum {
	GAME_STATE_ENGINE_INTRO,
	GAME_STATE_INTRO,
	GAME_STATE_MENU,
	GAME_STATE_INGAME
} gameState_e;

extern gameState_e gameState;

typedef struct gameInfo_s {
	char *name;
} gameInfo_t;

extern gameInfo_t gameInfo;

#include "Common.h"
#include "rData.h"
#include "rMath.h"
#include "rUI.h"
#include "rEntity.h"
#include "rEntityScriptWrapper.h"
#include "rTile.h"
#include "rMap.h"
#include "rMenu.h"
#include "rWorld.h"
#include "rPlayer.h"
#include "rScript.h"
#include "rBitmap.h"

extern rUI gUI;
extern rScript gScript;
extern rBitmap gBitmap;
extern rWorld gWorld;
extern rData gData;

extern rPlayer player;

void initialize_assets();

#endif