#pragma once
#include "stdafx.h"
#ifdef __USE_LUA
#define SCR_FUNC(x) int SCR_##x(lua_State *state)
#elif __USE_SQUIRREL
#define SCR_FUNC(x) SQInteger SCR_##x(HSQUIRRELVM v)
#else
#define SCR_FUNC(x) void SCR_##x()
#endif

class rScript
{
public:
	rScript();
	~rScript();
#ifdef __USE_LUA
	lua_State *luaState;
#endif
#ifdef __USE_SQUIRREL
	HSQUIRRELVM squirrelVM;
#endif
#ifdef __USE_ANGELSCRIPT
	asIScriptEngine *asEngine;
	CScriptBuilder asScriptBuilder;
	asIScriptContext * scriptContext;
#endif
	ALLEGRO_FILE * mainScript;
	int lastResult;
	bool hasCompileErrors = false;

	void LoadAssets();
	void ExecuteScript();
	void ExecuteLevelScript(char * name);
	void EntInteract(rEntity * parent);

	void ExecuteFunction(char *decl);

};
void ASMessageCallback(const asSMessageInfo * msg, void * param);
int ASIncludeCallback(const char * include, const char * from, CScriptBuilder * builder, void * userParam);
char * ReadScript(char* name);
#ifdef __USE_SQUIRREL
void SQPrintFunc(HSQUIRRELVM v, const SQChar * s, ...);

void SQErrorFunc(HSQUIRRELVM v, const SQChar * s, ...);

void SQCompilerError(HSQUIRRELVM v, const SQChar * desc, const SQChar * source, SQInteger line, SQInteger column);
#endif
//SCR_FUNC(Print);
void SCR_Print(string &txt);
void SCR_Ping();

void SCR_LoadMenu(string & txt);

void SCR_LoadMap(string & txt);

void SCR_OpenMenu(string & txt);

void SCR_OptimizeMap(string & txt);
