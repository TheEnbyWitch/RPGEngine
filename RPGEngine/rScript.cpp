#include "stdafx.h"
#include "rScript.h"


rScript::rScript()
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	lua_register(luaState, "print", SCR_Print);
}

rScript::~rScript()
{
	lua_close(luaState);
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
	char * script = new char[size+1];
	al_fread(scr, script, size);
	script[size] = '\0';
	return script;
}

void rScript::ExecuteScript()
{
	luaL_dostring(luaState, ReadScript("main.lua"));
	lua_getglobal(luaState, "main");
	lua_call(luaState, 0, 0);
	lua_pop(luaState, 1);
}

void rScript::ExecuteLevelScript(char * name)
{
	//char path[256];
	//sprintf(path, "maps/%s.lua", name);
	
	rpge_printf("Executing init(\"%s\")\n", name);
	char str[64];
	sprintf(str, "%s_init", name);
	lua_settop(luaState, 0);
	lua_getglobal(luaState, "init");
	lua_pushstring(luaState, name);
	lua_call(luaState, 1, 0);
	lua_pop(luaState, 1);
}

SCR_FUNC(Print)
{
	int args = lua_gettop(state);
	for (int i = 1; i <= args; i++)
	{
		rpge_printf("%s ", lua_tostring(state, i));
	}
	rpge_printf("\n");
	lua_pop(state, 1);
	return 0;
}