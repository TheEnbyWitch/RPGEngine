#pragma once
#include "stdafx.h"
typedef struct rScriptFunctionArg_s
{
	std::string type;
	std::string argName;
	std::string argDesc;
} rScriptFunctionArg_t;

rScriptFunctionArg_t ScrArg(std::string type, std::string argName, std::string argDesc = "");

std::vector<rScriptFunctionArg_t> ScrArgArray(int n, ...);

class rScriptFunction
{
public:
	rScriptFunction();
	~rScriptFunction();

	std::string GetScriptDeclaration();

	std::string returnType;
	std::string functionName;
	std::vector<rScriptFunctionArg_t> args;
	std::string description;
	std::string example;
};

class rScriptEngineHelper
{
public:
	rScriptEngineHelper();
	rScriptEngineHelper(asIScriptEngine * engine);
	~rScriptEngineHelper();

	void AddScriptFunction(std::string returnType, std::string functionName, std::vector<rScriptFunctionArg_t> args, const asSFuncPtr &funcPointer, asDWORD callConv, std::string desc = "<TODO>", std::string example = "<TODO>" );

	void WriteScriptDoc();
	void AssignEngine(asIScriptEngine * engine);
private:
	std::vector<rScriptFunction> scriptFuncs;
	asIScriptEngine * engine;
};

