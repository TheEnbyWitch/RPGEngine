#include "stdafx.h"

#ifndef __RPG_RSCRIPT
#define __RPG_RSCRIPT

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
	rScriptEngineHelper helper;
#endif
	ALLEGRO_FILE * mainScript;
	int lastResult;
	bool hasCompileErrors = false;

	void ExecuteScript();
	void ExecuteLevelScript(char * name);
	void EntInteract(rEntity * parent);
	void EntThink(rEntity * parent);

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
void SCR_Print(char *txt);
void SCR_Ping();

void SCR_LoadMenu(char *txt);

void SCR_LoadTexture(char *txt);

void SCR_LoadMap(char *txt);

void SCR_OpenMenu(char *txt);

void SCR_OptimizeMap(char *txt);
 
#endif