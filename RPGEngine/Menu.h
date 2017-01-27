#pragma once
#include "stdafx.h"

#define MENU_ACTION_FUNC(name) void name()
#define TYPEDEF_MENU_ACTION_FUNC(name) typedef MENU_ACTION_FUNC(name)

TYPEDEF_MENU_ACTION_FUNC(MenuAction_t);

typedef enum {
	ITEM_TYPE_WINDOW,
	ITEM_TYPE_TEXT,
	ITEM_TYPE_BUTTON,
	ITEM_TYPE_IMAGE
} rMenuItemType;

typedef struct rButtonAttr_s {
	int index;
	MenuAction_t *onClick = NULL;
} rButtonAttr_t;

typedef struct rImageAttr_s {
	ALLEGRO_BITMAP* image;
	rScaledRegion_t region;
} rImageAttr_t;

typedef struct rTextAttr_s {
	int align = ALLEGRO_ALIGN_LEFT;
} rTextAttr_t;

typedef struct rMenuItem_s {
	rRect_t rect = { 0, 0, 0, 0 };
	char * text;
	rMenuItemType type;
	ALLEGRO_COLOR color = al_map_rgb(0,128,255);
	ALLEGRO_COLOR text_color = al_map_rgb(255,255,255);

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

extern std::vector<class rMenu> Menus;
extern char * activeMenu;

class rMenu
{
public:
	rMenu();
	~rMenu();

	bool isActive;
	char * name;
	rMenuVars_t vars;
	std::vector<rMenuItem_t> items;

	void Draw();
	void Key(int keycode);
	void Frame();
};

