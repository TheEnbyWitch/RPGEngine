#include "stdafx.h"
#include "rScript.h"

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
	/*
	static int index = 0;
	static char string[10][4096];	// just in case
	char *result;
	result = string[index];
	index = (index + 1) & 9;
	strcpy(result, s);
	*/
	return va("%s", s);
}

void ConstructEnt(void*memory)
{
	new(memory) rEntityScriptWrapper();
}

void DestructEnt(void*memory)
{
	((rEntityScriptWrapper*)memory)->~rEntityScriptWrapper();
}

rScript::rScript()
{

}

rScript::~rScript()
{

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

void rScript::ExecuteScript()
{

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
	helper.AssignEngine(asEngine);
	helper.AddScriptFunction("void", "LoadMenu", ScrArgArray(1, ScrArg("string", "menuToLoad", "The menu file to load")), asFUNCTION(SCR_LoadMenu), asCALL_CDECL, "Loads the specified menufile", "LoadMenu(\"main\");");
	helper.AddScriptFunction("void", "print", ScrArgArray(1, ScrArg("string", "text", "The text to print")), asFUNCTION(SCR_Print), asCALL_CDECL, "Print a string to console", "print(\"Hello, world!\");");
	helper.AddScriptFunction("void", "LoadTexture", ScrArgArray(1, ScrArg("string", "textureToLoad", "The texture to load")), asFUNCTION(SCR_LoadTexture), asCALL_CDECL, "Loads the specified texture. It's possible to load a texture at will ingame, but it can cause a freeze.", "LoadTexture(\"guy.tga\");");
	helper.AddScriptFunction("void", "LoadMap", ScrArgArray(1, ScrArg("string", "mapToLoad", "The map to load")), asFUNCTION(SCR_LoadMap), asCALL_CDECL, "Loads the specified map into memory. Call in init().", "LoadMap(\"e1m1\");");
	helper.AddScriptFunction("void", "OpenMenu", ScrArgArray(1, ScrArg("string", "menuToOpen", "The menu to open")), asFUNCTION(SCR_OpenMenu), asCALL_CDECL, "Opens a previously loaded menu.", "OpenMenu(\"main\");");
	
	helper.AddScriptFunction("void", "RegisterSaveFieldInt", ScrArgArray(2, ScrArg("string", "field", "The field to store the value in"), ScrArg("int", "defaultValue", "The default value for the field")), 0, asCALL_CDECL, "Registers an int save field", "RegisterSaveFieldInt(\"aNumber\", 115);");
	helper.AddScriptFunction("void", "RegisterSaveFieldFloatt", ScrArgArray(2, ScrArg("string", "field", "The field to store the value in"), ScrArg("float", "defaultValue", "The default value for the field")), 0, asCALL_CDECL, "Registers a float save field", "RegisterSaveFieldFloat(\"aFloat\", 1.15);");
	helper.AddScriptFunction("void", "RegisterSaveFieldString", ScrArgArray(2, ScrArg("string", "field", "The field to store the value in"), ScrArg("string", "defaultValue", "The default value for the field")), 0, asCALL_CDECL, "Registers a string save field", "RegisterSaveFieldString(\"aString\", \"Hello, world!\");");

	r = asEngine->RegisterObjectType("rEntity", sizeof(rEntityScriptWrapper), asOBJ_VALUE | asOBJ_APP_CLASS_CDA);
	r = asEngine->RegisterObjectBehaviour("rEntity", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructEnt), asCALL_CDECL_OBJLAST);
	r = asEngine->RegisterObjectBehaviour("rEntity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructEnt), asCALL_CDECL_OBJLAST);

	// TODO: Object Methods in ScriptEngineHelper
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
	r = asEngine->RegisterObjectMethod("rEntity", "rEntity &opAssign(const rEntity &in)", asMETHOD(rEntityScriptWrapper, operator=), asCALL_THISCALL);

	asEngine->RegisterFuncdef("void EntityThinkCallback(rEntity ent)");
	helper.AddScriptFunction("void", "SetEntityThinkCallback", ScrArgArray(2, ScrArg("rEntity ", "ent", "The entity to set the callback for"), ScrArg("EntityThinkCallback @", "entityThinkCallbackFunction", "The function to call every frame")), asFUNCTION(SCR_SetEntityThinkCallback), asCALL_CDECL, "Sets the callback function for an entity to be called every frame");
	asEngine->RegisterFuncdef("void EntityInteractCallback(rEntity ent)");
	helper.AddScriptFunction("void", "SetEntityInteractCallback", ScrArgArray(2, ScrArg("rEntity ", "ent", "The entity to set the callback for"), ScrArg("EntityInteractCallback @", "entityInteractCallbackFunction", "The function to call when player interacts with this entity")), asFUNCTION(SCR_SetEntityInteractCallback), asCALL_CDECL, "Sets the callback function for an entity to be called when player interacts with it");


	helper.WriteScriptDoc();

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
}

void rScript::ExecuteFunction(char * decl)
{
	rpge_printf("[rScript] Executing %s\n", decl);
	scriptContext = asEngine->CreateContext();
	char str[64];
	sprintf(str, "void %s()", decl);
	if (scriptContext == 0)
		abort_game("Failed to create context!");
	asIScriptFunction *func = asEngine->GetModule(0)->GetFunctionByDecl(str);
	if (func == 0)
	{
		rpge_printf("[rScript] Couldn't find function declaraction for void %s()!\n", decl);
		return;
	}
	scriptContext->Prepare(func);
	scriptContext->Execute();
}

void rScript::Suspend()
{
}

void rScript::Resume()
{
}

void rScript::EntInteract(rEntity * parent)
{
	if (!parent->interactFunc) return;

	if(!parent->interactContext)
		parent->interactContext = asEngine->CreateContext();

	if (scriptContext == 0)
		abort_game("Failed to create context!");

	rEntityScriptWrapper t;
	t.SetEntityPointer(parent);

	parent->interactContext->Prepare(parent->interactFunc);
	parent->interactContext->SetArgObject(0, &t);
	parent->interactContext->Execute();
}

void rScript::EntThink(rEntity * parent)
{
	if (!parent->thinkFunc) return;

	if (!parent->thinkContext)
		parent->thinkContext = asEngine->CreateContext();

	if (!parent->thinkContext)
		abort_game("Failed to create context!");

	rEntityScriptWrapper t;
	t.SetEntityPointer(parent);

	parent->thinkContext->Prepare(parent->thinkFunc);
	parent->thinkContext->SetArgObject(0, &t);
	parent->thinkContext->Execute();
}

void SCR_Print(char *txt)
{
	rpge_printf("[DOOT] %s\n", txt);
}

void SCR_Ping()
{
	rpge_printf("[AS] Ping!\n");
}

void SCR_LoadMenu(char* txt)
{
	rpge_printf("[rScript] SCR_LoadMenu() - Loading menu %s\n", txt);
	gLoadQueue.AddToQueue(txt, ASSET_MENU);
}

void SCR_LoadTexture(char* txt)
{
	rpge_printf("[rScript] SCR_LoadTexture() - Loading texture %s\n", txt);
	gLoadQueue.AddToQueue(txt, ASSET_TEXTURE);
}


void SCR_LoadMap(char* txt)
{
	rpge_printf("[rScript] SCR_LoadMap() - Loading map %s\n", txt);
	auto l = gWorld.GetMapDependencies(txt);
	for (auto t : l)
	{
		gLoadQueue.AddToQueue(t.name, t.type);
	}
	gLoadQueue.AddToQueue(txt, ASSET_MAP);
}

void SCR_OpenMenu(char* txt)
{
	rpge_printf("[rScript] SCR_OpenMenu() - Opening menu %s\n", txt);
	strcpy(activeMenu, txt);
}

void SCR_SetEntityInteractCallback(rEntityScriptWrapper ent, asIScriptFunction * callback)
{
	
	if (!ent.HasAnAssignedEnt())
	{
	rpge_printf("[rScript] Tried to assign an interact callback for a null entity\n");
	return;
	}
	
	ent.GetEntityPointer()->interactFunc = callback;
}

void SCR_SetEntityThinkCallback(rEntityScriptWrapper ent, asIScriptFunction * callback)
{
	
	if (!ent.HasAnAssignedEnt())
	{
		rpge_printf("[rScript] Tried to assign a think callback for a null entity\n");
		return;
	}
	
	ent.GetEntityPointer()->thinkFunc = callback;
}

void SCR_Dialogue(char* txt)
{
	abort_game("SCR_Dialogue() is not implemented yet!");
	
}