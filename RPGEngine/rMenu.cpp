#include "rMenu.h"
#include "stdafx.h"

#define MENU_VERBOSE if(gConsole.GetDvar("menu_verbose")->ToInt())

std::vector<class rMenu> Menus;
char activeMenu[32]{ 'n','u','l','l'};
namespace rColor
{
	ALLEGRO_COLOR toAllegro(rColor_t clr)
	{
		return al_map_rgba(clr.r, clr.g, clr.b, clr.a);
	}
}

rMenu::rMenu()
{
}


rMenu::~rMenu()
{
}

void rMenu::Draw()
{
	for(int i = 0; i < this->items.size(); i++)
	{
		rMenuItem_t *item = &items[i];
		switch (item->type)
		{
		case ITEM_TYPE_WINDOW:
			gUI.DrawColoredWindow(item->rect.X, item->rect.Y, item->rect.W, item->rect.H, rColor::toAllegro(item->color));
			break;
		case ITEM_TYPE_TEXT:
			gUI.DrawColoredText(item->text, item->rect.X, item->rect.Y, item->rect.W, rColor::toAllegro(item->text_color), item->textAttributes.align);
			break;
		case ITEM_TYPE_BUTTON:
			gUI.DrawColoredMenuOption(item->buttonAttributes.index, item->rect.X, item->rect.Y, item->rect.W, item->rect.H, item->text, rColor::toAllegro(item->color), this);
			break;
		}
	}
}

void rMenu::Key(int keycode)
{
	if (!showCon && isActive)
	{
		if (keycode == ALLEGRO_KEY_DOWN)
		{
			this->vars.selectedIndex++;
		}
		if (keycode == ALLEGRO_KEY_UP)
		{
			this->vars.selectedIndex--;
		}
		if (keycode == ALLEGRO_KEY_ENTER)
		{
			gSound.PlaySample("ui/enter.wav");
			for (int i = 0; i < this->items.size(); i++)
			{
				rMenuItem_t *item = &items[i];
				if (item->type != ITEM_TYPE_BUTTON) continue;
				if (item->buttonAttributes.index == this->vars.selectedIndex) rMenu::ExecuteAction(item->buttonAttributes.onClick, item->buttonAttributes.onClickArgs);
			}
		}
	}
}

void rMenu::Frame()
{
	for (int i = 0; i < maxMenuIndex; i++)
	{
		if (this->vars.selectedIndex == i)
		{
			this->vars.menuIndexSelectFrac[i] += 0.1f;
		}
		else
		{
			this->vars.menuIndexSelectFrac[i] -= 0.1f;
		}
		this->vars.menuIndexSelectFrac[i] = __min(__max(0.0f, this->vars.menuIndexSelectFrac[i]), 1.0f); // avoid conflicts with the global one
	}
}

rMenu rMenu::ReadMenu(char * name)
{
	DrawLoadWindow("Reading menu...", 1);
	rMenu result;
	ALLEGRO_FILE * menufile;
	char * filename = new char[256];
	sprintf(filename, "menu/%s.rmenu", name);
	MENU_VERBOSE rpge_printf("[rMenu] Opening menu: %s\n", filename);
	menufile = al_fopen(filename, "rb");
	char magic[6];
	al_fread(menufile, magic, 6);
	MENU_VERBOSE rpge_printf("[rMenu] menu magic: %s\n", magic);
	al_fread(menufile, result.name, sizeof(result.name));
	int itemCount;
	al_fread(menufile, &itemCount, sizeof(int));
	MENU_VERBOSE rpge_printf("[rMenu] item count: %d\n", itemCount);
	for (int i = 0; i < itemCount; i++)
	{
		rMenuItem_t item;
		al_fread(menufile, &item, sizeof(rMenuItem_t));
		MENU_VERBOSE rpge_printf("[rMenu] Processing item with text %s\n", item.text);
		result.items.push_back(item);
	}
	al_fclose(menufile);
	return result;
}

void rMenu::ExecuteAction(char * func, char * arg)
{
	for (int i = 0; i < sizeof(menuActionFuncs) / sizeof(rMenuActionFunc_t); i++)
	{
		if (strcmp(func, menuActionFuncs[i].name) == 0)
		{
			menuActionFuncs[i].func(arg);
		}
	}
}

MENU_ACTION_FUNC(M_OpenMenu)
{
	rpge_printf("[rMenu] M_OpenMenu() - Opening menu %s\n", argument);
	strcpy(activeMenu, argument);
}

MENU_ACTION_FUNC(M_QuitGame)
{
	rpge_printf("[rMenu] M_QuitGame() - Quitting game\n");
	exit(0);
}

MENU_ACTION_FUNC(M_StartGame)
{
	rpge_printf("[rMenu] M_StartGame() - Starting game\n");
	gameState = GAME_STATE_INGAME;
	gSound.PlayMusic("test.flac");
}

MENU_ACTION_FUNC(M_ExecScript)
{
	rpge_printf("[rMenu] M_ExecScript() - Executing script function %s()\n", argument);
	gScript.ExecuteFunction(argument);
}