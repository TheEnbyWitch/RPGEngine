#pragma once
#ifndef _RPG_COMPILER
#include "stdafx.h"
#endif

#define MENU_ACTION_FUNC(name) void name()
#define TYPEDEF_MENU_ACTION_FUNC(name) typedef MENU_ACTION_FUNC(name)

TYPEDEF_MENU_ACTION_FUNC(MenuAction_t);

#ifdef _RPG_COMPILER
typedef struct rColor_s {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} rColor_t;
#define ALLEGRO_COLOR rColor_t
#define al_map_rgb(r,g,b) ALLEGRO_COLOR{r,g,b,255}
#define al_map_rgba(r,g,b,a) ALLEGRO_COLOR{r,g,b,a}
#undef maxMenuIndex
#define maxMenuIndex 128
typedef struct rRect_s {
	int X;
	int Y;
	int W;
	int H;
} rRect_t;

typedef struct rScaledRegion_s {
	int sourceX;
	int sourceY;
	int sourceW;
	int sourceH;
	int destinationX;
	int destinationY;
	int destinationW;
	int destinationH;
} rScaledRegion_t;
#else
typedef struct rColor_s {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} rColor_t;
#endif

const char * rMenuMagic = "rMENU";

typedef enum {
	ITEM_TYPE_WINDOW,
	ITEM_TYPE_TEXT,
	ITEM_TYPE_BUTTON,
	ITEM_TYPE_IMAGE
} rMenuItemType;

typedef struct rButtonAttr_s {
	int index;
	char onClick[32];
	char onClickArgs[256];
	MenuAction_t *onClickFunc = NULL;
} rButtonAttr_t;

typedef struct rImageAttr_s {
	char imagePath[256];
#ifndef _RPG_COMPILER
	//ALLEGRO_BITMAP* image;
#endif
	rScaledRegion_t region;
} rImageAttr_t;

typedef struct rTextAttr_s {
#ifdef _RPG_COMPILER
	int align = 0;
#else
	int align = ALLEGRO_ALIGN_LEFT;
#endif
	
} rTextAttr_t;

typedef struct rMenuItem_s {
	rRect_t rect = { 0, 0, 0, 0 };
	char text[1024];
	rMenuItemType type;
	rColor_t color;
	rColor_t text_color;

	//TYPE SPECIFIC
	// -- BUTTON
	rButtonAttr_t buttonAttributes;
	// -- IMAGE
	rImageAttr_t imageAttributes;
	// -- TEXT
	rTextAttr_t textAttributes;
} rMenuItem_t;

typedef struct rMenuVars_s {
	int selectedIndex = 0;
	float menuIndexSelectFrac[maxMenuIndex];
} rMenuVars_t;
#ifndef _RPG_COMPILER
extern std::vector<class rMenu> Menus;
extern char * activeMenu;
#endif

class rMenu
{
public:
	rMenu();
	~rMenu();

	bool isActive;
	char name[16];
	rMenuVars_t vars;
	std::vector<rMenuItem_t> items;

	void Draw();
	void Key(int keycode);
	void Frame();

	static rMenu ReadMenu(char * filename);
};

