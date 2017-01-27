#include "stdafx.h"
#include "Menu.h"


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
			gUI.DrawMenuOption(item.buttonAttributes.index, item.rect.X, item.rect.Y, item.rect.W, item.rect.H, item.text, this);
			break;
		}
	}
}

void rMenu::Key(int keycode)
{
	if (!showCon)
	{
		if (keycode == ALLEGRO_KEY_DOWN)
		{
			selectedIndex++;
		}
		if (keycode == ALLEGRO_KEY_UP)
		{
			selectedIndex--;
		}
	}
}

void rMenu::Frame()
{
	for (int i = 0; i < maxMenuIndex; i++)
	{
		if (menuIndex == i)
		{
			this->menuIndexSelectFrac[i] += 0.1f;
		}
		else
		{
			this->menuIndexSelectFrac[i] -= 0.1f;
		}
		this->menuIndexSelectFrac[i] = __min(__max(0.0f, this->menuIndexSelectFrac[i]), 1.0f); // avoid conflicts with the global one
	}
}
