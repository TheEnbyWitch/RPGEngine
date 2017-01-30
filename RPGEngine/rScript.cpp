#include "stdafx.h"
#include "rScript.h"


rScript::rScript()
{
}


rScript::~rScript()
{
}

char * rScript::ReadScript(char * name)
{
	ALLEGRO_FILE * scr;
	scr = al_fopen(name, "rb");
	if (scr == NULL)
	{
		char error[512];
		sprintf(error, "Failed reading script!\n%s", name);
		abort_game(error);
		return nullptr;
	}
	int size = al_fsize(scr);
	char * script = new char[size];
	al_fread(scr, script, size);
	return script;
}

void rScript::ExecuteScript()
{
}

void SCR_Print(std::string txt)
{
	rpge_printf("%s\n", txt.c_str());
}
