#pragma once
#include "stdafx.h"
#ifdef __USE_LUA
#define SCR_FUNC(x) int SCR_##x(lua_State *state)
#else
#define SCR_FUNC(x) SQInteger SCR_##x(HSQUIRRELVM v)
#endif

class rScript
{
public:
	rScript();
	~rScript();
#ifdef __USE_LUA
	lua_State *luaState;
#endif
	HSQUIRRELVM squirrelVM;
	ALLEGRO_FILE * mainScript;
	int lastResult;

	void ExecuteScript();
	void ExecuteLevelScript(char * name);

};

char * ReadScript(char* name);

void SQPrintFunc(HSQUIRRELVM v, const SQChar * s, ...);

void SQErrorFunc(HSQUIRRELVM v, const SQChar * s, ...);

void SQCompilerError(HSQUIRRELVM v, const SQChar * desc, const SQChar * source, SQInteger line, SQInteger column);

SCR_FUNC(Print);