#include "stdafx.h"
#include "rScript.h"


void SQPrintFunc(HSQUIRRELVM SQ_UNUSED_ARG(v), const SQChar *s, ...)
{
	va_list vl;
	va_start(vl, s);
	rpge_printf(s, vl);
	va_end(vl);
}

void SQErrorFunc(HSQUIRRELVM SQ_UNUSED_ARG(v), const SQChar *s, ...)
{
	va_list vl;
	va_start(vl, s);
	char errstr[1024];
	vsprintf(errstr, s, vl);
	va_end(vl);
}

void SQCompilerError(HSQUIRRELVM v, const SQChar * desc, const SQChar * source, SQInteger line, SQInteger column)
{
	char errstr[1024];
	sprintf(errstr, "%s:%d:%d\n%s", source, line, column, desc);
	abort_game(errstr);
}

SQInteger SQRegisterFunc(HSQUIRRELVM v, SQFUNCTION f, const char *fname)
{
	sq_pushroottable(v);
	sq_pushstring(v, fname, -1);
	sq_newclosure(v, f, 0); //create a new function
	sq_newslot(v, -3, SQFalse);
	sq_pop(v, 1); //pops the root table    
	return 0;
}

rScript::rScript()
{
#ifdef __USE_LUA
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	lua_register(luaState, "print", SCR_Print);
#endif
	squirrelVM = sq_open(1024);
	sq_setprintfunc(squirrelVM, SQPrintFunc, SQErrorFunc);
	sq_pushroottable(squirrelVM);
	sq_setcompilererrorhandler(squirrelVM, SQCompilerError);

	sqstd_register_mathlib(squirrelVM);
	sqstd_register_stringlib(squirrelVM);

	SQRegisterFunc(squirrelVM, SCR_Print, "print_kek");
}

rScript::~rScript()
{
#ifdef __USE_LUA
	lua_close(luaState);
#endif
	sq_close(squirrelVM);
}

char * ReadScript(char * name)
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

SQInteger file_lexfeedASCII(SQUserPointer file) {
	int ret;
	char c;
	ret = al_fread((ALLEGRO_FILE *)file, &c, 1);//fread(&c, sizeof(c), 1, (FILE *)file);
	if(ret > 0)
		return c;
	return 0;
}

void rScript::ExecuteScript()
{
	mainScript = al_fopen("game.doot", "rb");
	if (!mainScript)
	{
		abort_game("game.doot script not found!");
	}
	sq_compile(squirrelVM, file_lexfeedASCII, mainScript, "game.doot", 1);
	sq_pushroottable(squirrelVM);
	sq_pushstring(squirrelVM, "main", -1);
	if (!SQ_SUCCEEDED(sq_get(squirrelVM, -2)))
	{
		abort_game("Couldn't find main()!");
	}
	sq_pushroottable(squirrelVM);
	sq_pushinteger(squirrelVM, 1);
	sq_call(squirrelVM, 2, SQFalse, SQFalse);
	sq_pop(squirrelVM, 2);
#ifdef __USE_LUA
	luaL_dostring(luaState, ReadScript("main.lua"));
	lua_getglobal(luaState, "main");
	lua_call(luaState, 0, 0);
	lua_pop(luaState, 1);
#endif
	
}

void rScript::ExecuteLevelScript(char * name)
{
#ifdef __USE_LUA
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
#endif
}

SCR_FUNC(Print)
{
#ifdef __USE_LUA
	int args = lua_gettop(state);
	for (int i = 1; i <= args; i++)
	{
		rpge_printf("%s ", lua_tostring(state, i));
	}
	rpge_printf("\n");
	lua_pop(state, 1);
	return 0;
#endif
	rpge_printf("SCR_Print was called\n");
	SQInteger nargs = sq_gettop(v); //number of arguments
	for (SQInteger n = 1; n <= nargs; n++)
	{
		SQChar * res[512];
		sq_getstring(v, n, (const SQChar **)&res);
		rpge_printf("%s ", res);
	}
	rpge_printf("\n");
	return 0;
}