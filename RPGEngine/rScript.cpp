#include "stdafx.h"
#include "rScript.h"

#ifdef __USE_SQUIRREL
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
#endif

#ifdef __USE_ANGELSCRIPT
void ASMessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	rpge_printf("[Angel] %s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);

	if (msg->type == asMSGTYPE_ERROR)
	{
		((rScript *)param)->hasCompileErrors = true;
	}
}

int ASIncludeCallback(const char *include, const char *from, CScriptBuilder *builder, void *userParam)
{
	rpge_printf("[Angel] %s : Including %s\n", from, include);
	return builder->AddSectionFromMemory((char *)include, ReadScript((char *)include));
}

char* ASStringFactory(unsigned int byteLength, const char *s)
{
	static int index = 0;
	static char string[10][4096];	// just in case
	char *result;
	result = string[index];
	index = (index + 1) & 9;
	strcpy(result, s);
	return result;
}

void ConstructEnt(void*memory)
{
	new(memory) rEntityScriptWrapper();
}

void DestructEnt(void*memory)
{
	((rEntityScriptWrapper*)memory)->~rEntityScriptWrapper();
}

#endif

rScript::rScript()
{
#ifdef __USE_LUA
	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	lua_register(luaState, "print", SCR_Print);
#endif
#ifdef __USE_SQUIRREL
	squirrelVM = sq_open(1024);
	sq_setprintfunc(squirrelVM, SQPrintFunc, SQErrorFunc);
	sq_pushroottable(squirrelVM);
	sq_setcompilererrorhandler(squirrelVM, SQCompilerError);

	sqstd_register_mathlib(squirrelVM);
	sqstd_register_stringlib(squirrelVM);

	SQRegisterFunc(squirrelVM, SCR_Print, "print_kek");
#endif
#ifdef __USE_ANGELSCRIPT
	
#endif
}

rScript::~rScript()
{
#ifdef __USE_LUA
	lua_close(luaState);
#endif
#ifdef __USE_SQUIRREL
	sq_close(squirrelVM);
#endif
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
#ifdef __USE_SQUIRREL
SQInteger file_lexfeedASCII(SQUserPointer file) {
	int ret;
	char c;
	ret = al_fread((ALLEGRO_FILE *)file, &c, 1);//fread(&c, sizeof(c), 1, (FILE *)file);
	if(ret > 0)
		return c;
	return 0;
}
#endif
void rScript::ExecuteScript()
{
#ifdef __USE_SQUIRREL
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
#endif
#ifdef __USE_LUA
	luaL_dostring(luaState, ReadScript("main.lua"));
	lua_getglobal(luaState, "main");
	lua_call(luaState, 0, 0);
	lua_pop(luaState, 1);
#endif
	rpge_printf("Initializing AngelScript\n");
	asEngine = asCreateScriptEngine();
	int r = asEngine->SetMessageCallback(asFUNCTION(ASMessageCallback), this, asCALL_CDECL);

	RegisterScriptArray(asEngine, true);
	//RegisterStdString(asEngine);
	//RegisterScriptMath(asEngine);
	RegisterScriptHandle(asEngine);
	RegisterScriptWeakRef(asEngine);
	r = asEngine->RegisterObjectType("string", sizeof(char*), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<char*>());
	r = asEngine->RegisterStringFactory("string", asFUNCTION(ASStringFactory), asCALL_CDECL);
	r = asScriptBuilder.StartNewModule(asEngine, 0);
	asScriptBuilder.SetIncludeCallback(&ASIncludeCallback, nullptr);
#ifdef _DEBUG
	asScriptBuilder.DefineWord("DEBUG");
#endif

	r = asEngine->RegisterGlobalFunction("void print(string txt)", asFUNCTION(SCR_Print), asCALL_CDECL);
	r = asEngine->RegisterGlobalFunction("void ping()", asFUNCTION(SCR_Ping), asCALL_CDECL);
	r = asEngine->RegisterGlobalFunction("void LoadMenu(string txt)", asFUNCTION(SCR_LoadMenu), asCALL_CDECL);
	r = asEngine->RegisterGlobalFunction("void LoadMap(string txt)", asFUNCTION(SCR_LoadMap), asCALL_CDECL);
	r = asEngine->RegisterGlobalFunction("void OpenMenu(string txt)", asFUNCTION(SCR_OpenMenu), asCALL_CDECL);

	r = asEngine->RegisterObjectType("rEntity", sizeof(rEntityScriptWrapper), asOBJ_VALUE);
	r = asEngine->RegisterObjectBehaviour("rEntity", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructEnt), asCALL_CDECL_OBJLAST);
	r = asEngine->RegisterObjectBehaviour("rEntity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructEnt), asCALL_CDECL_OBJLAST);

	r = asEngine->RegisterObjectMethod("rEntity", "void GetEnt(string id)", asMETHOD(rEntityScriptWrapper, GetEnt), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "void Spawn(string id)", asMETHOD(rEntityScriptWrapper, Spawn), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "bool HasAnAssignedEnt()", asMETHOD(rEntityScriptWrapper, HasAnAssignedEnt), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "bool Move(int x, int y)", asMETHOD(rEntityScriptWrapper, Move), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "void ChangeDirection(int target)", asMETHOD(rEntityScriptWrapper, ChangeDirection), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "void SetImage(string path)", asMETHOD(rEntityScriptWrapper, SetImage), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "int GetIntValue(string key)", asMETHOD(rEntityScriptWrapper, GetIntValue), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "bool GetBoolValue(string key)", asMETHOD(rEntityScriptWrapper, GetBoolValue), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "void GetIntValue(string key, int value)", asMETHOD(rEntityScriptWrapper, SetIntValue), asCALL_THISCALL);
	r = asEngine->RegisterObjectMethod("rEntity", "void SetBoolValue(string key, bool value)", asMETHOD(rEntityScriptWrapper, SetBoolValue), asCALL_THISCALL);

	asScriptBuilder.AddSectionFromMemory("main.doot", ReadScript("main.doot"));
	asScriptBuilder.BuildModule();
	if (hasCompileErrors)
		abort_game("AngelScript encountered errors when compiling. Check log for more info.");

	scriptContext = asEngine->CreateContext();
	if (scriptContext == 0)
		abort_game("Failed to create context!");
	asIScriptFunction *func = asEngine->GetModule(0)->GetFunctionByDecl("void init()");
	
	if (func == 0)
		abort_game("Main game script doesn't have void init()!");
	scriptContext->Prepare(func);
	scriptContext->Execute();
	
	//scriptContext->Release();

	scriptContext = asEngine->CreateContext();
	if (scriptContext == 0)
		abort_game("Failed to create context!");
	func = asEngine->GetModule(0)->GetFunctionByDecl("void main()");
	if (func == 0)
		abort_game("Main game script doesn't have void main()!");
	scriptContext->Prepare(func);
	scriptContext->Execute();
	//scriptContext->Release();
}

void rScript::ExecuteLevelScript(char * name)
{
	rpge_printf("[rScript] Executing %s_init\n", name);
	scriptContext = asEngine->CreateContext();
	char str[64];
	sprintf(str, "void %s_init()", name);
	if (scriptContext == 0)
		abort_game("Failed to create context!");
	asIScriptFunction *func = asEngine->GetModule(0)->GetFunctionByDecl(str);
	if (func == 0)
		abort_game("Couldn't find init function for level!");
	scriptContext->Prepare(func);
	scriptContext->Execute();

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

void rScript::EntInteract(rEntity * parent)
{
	//parent->uniqueID
	char scriptEntID[16];
	strcpy(scriptEntID, parent->uniqueID);
	rpge_printf("[rScript] Executing %s_interact\n", scriptEntID);
	scriptContext = asEngine->CreateContext();
	char str[64];
	sprintf(str, "void %s_interact()", scriptEntID);
	if (scriptContext == 0)
		abort_game("Failed to create context!");
	asIScriptFunction *func = asEngine->GetModule(0)->GetFunctionByDecl(str);
	if (func == 0)
	{
		rpge_printf("[rScript] Couldn't find interact function for %s!\n", scriptEntID);
		return;
	}
	scriptContext->Prepare(func);
	scriptContext->Execute();
}

void rScript::EntThink(rEntity * parent)
{
	if (!parent->hasThinkFunc) return;
	//parent->uniqueID
	char scriptEntID[16];
	strcpy(scriptEntID, parent->uniqueID);
	//rpge_printf("[rScript] Executing %s_think\n", scriptEntID);
	if (parent->thinkContext == 0)
		parent->thinkContext = asEngine->CreateContext();
	char str[64];
	sprintf(str, "void %s_think()", scriptEntID);
	if (parent->thinkContext == 0)
		abort_game("Failed to create context!");
	if (parent->thinkFunc == 0)
	{
		parent->thinkFunc = asEngine->GetModule(0)->GetFunctionByDecl(str);
	}
	if (parent->thinkFunc == 0)
	{
		parent->hasThinkFunc = false;
		rpge_printf("[rScript] Couldn't find think function for %s!\n", scriptEntID);
		return;
	}
	if (parent->thinkFunc != 0)
	{
		parent->thinkContext->Prepare(parent->thinkFunc);
		parent->thinkContext->Execute();
	}
}

void SCR_Print(char *txt)
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
#ifdef __USE_SQUIRREL
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
#endif
	rpge_printf("[DOOT] %s\n", txt);
}

void SCR_Ping()
{
	rpge_printf("[AS] Ping!\n");
}

void SCR_LoadMenu(char* txt)
{
	rpge_printf("[rScript] SCR_LoadMenu() - Loading menu %s\n", txt);
	Menus.push_back(rMenu::ReadMenu(txt));
}

void SCR_LoadMap(char* txt)
{
	rpge_printf("[rScript] SCR_LoadMap() - Loading map %s\n", txt);
	gWorld.LoadMap(txt);
}

void SCR_OpenMenu(char* txt)
{
	rpge_printf("[rScript] SCR_OpenMenu() - Opening menu %s\n", txt);
	strcpy(activeMenu, txt);
}