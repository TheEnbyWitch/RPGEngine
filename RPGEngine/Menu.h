#pragma once
#include "stdafx.h"

#define MENU_ACTION_FUNC(name) void name()
#define TYPEDEF_MENU_ACTION_FUNC(name) typedef MENU_ACTION_FUNC(name)

TYPEDEF_MENU_ACTION_FUNC(MenuAction_t);

enum {
	ITEM_TYPE_WINDOW,
	ITEM_TYPE_TEXT,
	ITEM_TYPE_BUTTON,
	ITEM_TYPE_IMAGE
} rMenuItemType;

typedef struct rButtonAttr_s {
	int index;
	MenuAction_t *onClick;
} rButtonAttr_t;

typedef struct rImageAttr_s {
	ALLEGRO_BITMAP* image;
	rScaledRegion_t region;
} rImageAttr_t;

typedef struct rMenuItem_s {
	rRect_t rect;
	char * text;
	rMenuItemType type;
	ALLEGRO_COLOR color;
	ALLEGRO_COLOR text_color;

	//TYPE SPECIFIC
	// -- BUTTON
	rButtonAttr_t buttonAttributes;
	// -- IMAGE
	rImageAttr_t imageAttributes;
} rMenuItem_t;

class rMenu
{
public:
	rMenu();
	~rMenu();

	bool isActive;
	char * name;

	int selectedIndex;
	std::vector<rMenuItem_t> items;

	void Draw();
	void Key(int keycode);
};

