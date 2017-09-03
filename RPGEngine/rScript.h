#include "stdafx.h"

#ifndef __RPG_RSCRIPT
#define __RPG_RSCRIPT

class rScript
{
public:
	rScript();
	~rScript();

	asIScriptEngine *asEngine = NULL;
	CScriptBuilder asScriptBuilder;
	asIScriptContext * scriptContext = NULL;
	rScriptEngineHelper helper;

	ALLEGRO_FILE * mainScript;
	int lastResult;
	bool hasCompileErrors = false;

	void ExecuteScript();
	void ExecuteLevelScript(char * name);
	void EntInteract(rEntity * parent);
	void EntThink(rEntity * parent);

	void ExecuteFunction(char *decl);

	void Suspend();
	void Resume();

	bool scriptEngineSuspended = false;

};
void ASMessageCallback(const asSMessageInfo * msg, void * param);
int ASIncludeCallback(const char * include, const char * from, CScriptBuilder * builder, void * userParam);
char * ReadScript(char* name);

void SCR_Print(char *txt);
void SCR_Ping();

void SCR_LoadMenu(char *txt);

void SCR_LoadTexture(char *txt);

void SCR_LoadMap(char *txt);

void SCR_OpenMenu(char *txt);

void SCR_OptimizeMap(char *txt);

void SCR_SetEntityInteractCallback(rEntityScriptWrapper &ent, asIScriptFunction * callback);
void SCR_SetEntityThinkCallback(rEntityScriptWrapper &ent, asIScriptFunction * callback);
 
#endif