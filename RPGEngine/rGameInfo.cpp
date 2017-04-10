#include "stdafx.h"
#include "rGameInfo.h"
#include "RapidXML\rapidxml.hpp"

using namespace rapidxml;
rGameInfo gGameInfo;

char * ReadGameInfo(char * name)
{
	ALLEGRO_FILE * scr;
	scr = al_fopen(name, "rb");
	if (scr == NULL)
	{
		char error[512];
		sprintf(error, "Failed reading %s!\n", name);
		abort_game(error);
		return nullptr;
	}
	int size = al_fsize(scr);
	char * script = new char[size + 1];
	al_fread(scr, script, size);
	script[size] = '\0';
	return script;
}

rGameInfo::rGameInfo()
{
}


rGameInfo::~rGameInfo()
{
}

void rGameInfo::Init()
{
	xml_document<> doc;
	doc.parse<0>(ReadGameInfo("gameInfo.xml"));

	// Process game info
	xml_node<> *gameNode = doc.first_node("game");
	strcpy(gameName, gameNode->first_attribute("name")->value());
	strcpy(gameCompany, gameNode->first_attribute("company")->value());
	uint16_t id = 1;
	rawMenuInfo.push_back({ ALLEGRO_START_OF_MENU("&Debug", id++), "" });
	rawMenuInfo.push_back({ ALLEGRO_START_OF_MENU("&Show", id++), "" });
	rawMenuInfo.push_back({ { "FPS", id++, 0, NULL }, "" });
	rawMenuInfo.push_back({ { "TOD Debug", id++, 0, NULL },"" });
	rawMenuInfo.push_back({ { "Player Pos", id++, 0, NULL },"" });
	rawMenuInfo.push_back({ ALLEGRO_END_OF_MENU,"" });
	rawMenuInfo.push_back({ { NULL,         (uint16_t )-1, 0, NULL },"" });
	rawMenuInfo.push_back({ { "E&xit", id++, 0, NULL },"" });
	rawMenuInfo.push_back({ ALLEGRO_END_OF_MENU,"" });
	// Process debug menu
	xml_node<> *menuNode = gameNode->first_node("debugMenu");
	xml_node<> *menuItemNode = menuNode->first_node("menuItem");

	xml_node<> *dummynode = new xml_node<>(rapidxml::node_type::node_data);
	xml_attribute<> *dummyattr = new xml_attribute<>();

#define RETURN_VALID_ATTR(c) (c ? c : dummyattr)->value()
#define RETURN_VALID(c) (c ? c : dummynode)->value()
#define MENU_FLAG(attr, i) (strcmp(RETURN_VALID_ATTR(menuItemNode->first_attribute(attr)), "true") == 0 ? i : 0)
	while (menuItemNode)
	{
		rGMenuItem_t item;
		if (strcmp("startMenu", RETURN_VALID_ATTR(menuItemNode->first_attribute("type"))) == 0)
		{
			item.MI = { va("%s->", RETURN_VALID(menuItemNode)), id, MENU_FLAG("disabled", ALLEGRO_MENU_ITEM_DISABLED) | MENU_FLAG("checkbox", ALLEGRO_MENU_ITEM_CHECKBOX) | MENU_FLAG("checked", ALLEGRO_MENU_ITEM_CHECKED) , NULL };
			strcpy(item.scriptFunction, RETURN_VALID_ATTR(menuItemNode->first_attribute("scriptFunction")));
		}
		else if (strcmp("separator", RETURN_VALID_ATTR(menuItemNode->first_attribute("type"))) == 0)
		{
			item.MI = { NULL,         (uint16_t)-1, 0, NULL };
		}
		else if (strcmp("endMenu", RETURN_VALID_ATTR(menuItemNode->first_attribute("type"))) == 0)
		{
			item.MI = ALLEGRO_END_OF_MENU;
		}
		else 
		{
			item.MI = { RETURN_VALID(menuItemNode), id, MENU_FLAG("disabled", ALLEGRO_MENU_ITEM_DISABLED) | MENU_FLAG("checkbox", ALLEGRO_MENU_ITEM_CHECKBOX) | MENU_FLAG("checked", ALLEGRO_MENU_ITEM_CHECKED) , NULL };
			strcpy(item.scriptFunction, RETURN_VALID_ATTR(menuItemNode->first_attribute("scriptFunction")));
		}
		id++;
		rawMenuInfo.push_back(item);
		menuItemNode = menuItemNode->next_sibling("menuItem");
	}
	rawMenuInfo.push_back({ ALLEGRO_START_OF_MENU("&Help", id++),"" });
	rawMenuInfo.push_back({ { "&About", id, 0, NULL },"" }); aboutID = id;
	rawMenuInfo.push_back({ ALLEGRO_END_OF_MENU,"" });
	rawMenuInfo.push_back({ ALLEGRO_END_OF_MENU, "" });
}

ALLEGRO_MENU_INFO * rGameInfo::GetMenuInfo()
{
	if (rawMenuInfoArray != NULL)
	{
		delete[] rawMenuInfoArray;
		rawMenuInfoArray = NULL;
	}
	rawMenuInfoArray = new ALLEGRO_MENU_INFO[rawMenuInfo.size()];
	for (int i = 0; i < rawMenuInfo.size(); i++)
		rawMenuInfoArray[i] = rawMenuInfo[i].MI;
	return rawMenuInfoArray;
}

char * rGameInfo::GetScriptFuncForItem(int id)
{
	return nullptr;
}
