#include "stdafx.h"
#include "Menu.h"

std::vector<class rMenu> Menus;
char * activeMenu;


rMenu::rMenu()
{
}


rMenu::~rMenu()
{
}

void rMenu::Draw()
{
	for each (rMenuItem_t item in items)
	{
		switch (item.type)
		{
		case ITEM_TYPE_WINDOW:
			gUI.DrawColoredWindow(item.rect.X, item.rect.Y, item.rect.W, item.rect.H, item.color);
			break;
		case ITEM_TYPE_TEXT:
			gUI.DrawColoredText(item.text, item.rect.X, item.rect.Y, item.rect.W, item.text_color, item.textAttributes.align);
			break;
		case ITEM_TYPE_BUTTON:
			gUI.DrawColoredMenuOption(item.buttonAttributes.index, item.rect.X, item.rect.Y, item.rect.W, item.rect.H, item.text, item.color, this);
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
