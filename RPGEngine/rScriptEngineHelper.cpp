#include "stdafx.h"
#include "rScriptEngineHelper.h"


rScriptEngineHelper::rScriptEngineHelper()
{
}

rScriptEngineHelper::rScriptEngineHelper(asIScriptEngine * engine)
{
	AssignEngine(engine);
}


rScriptEngineHelper::~rScriptEngineHelper()
{
}

void rScriptEngineHelper::AddScriptFunction(std::string returnType, std::string functionName, std::vector<rScriptFunctionArg_t> args, const asSFuncPtr &funcPointer, asDWORD callConv, std::string desc, std::string example)
{
	rScriptFunction f;
	f.returnType = returnType;
	f.functionName = functionName;
	for (auto arg : args)
	{
		f.args.push_back(arg);
	}

	f.description = desc;
	f.example = example;
	scriptFuncs.push_back(f);
	engine->RegisterGlobalFunction(f.GetScriptDeclaration().c_str(), funcPointer, callConv);
}

void rScriptEngineHelper::WriteScriptDoc()
{
	nlohmann::json j;
	FILE *f;
	fopen_s(&f, "scriptdoc_data.json", "wb");
	for (auto f : scriptFuncs)
	{
		j[f.functionName]["decl"] = f.GetScriptDeclaration();
		j[f.functionName]["desc"] = f.description;
		j[f.functionName]["example"] = f.example;
		j[f.functionName]["returnType"] = f.returnType;
		for (auto a : f.args)
		{
			j[f.functionName]["args"][a.argName]["type"] = a.type;
			j[f.functionName]["args"][a.argName]["desc"] = a.argDesc;
		}
	}
	fwrite(j.dump(4).c_str(), sizeof(char), strlen(j.dump(4).c_str()), f);
	fclose(f);
}

void rScriptEngineHelper::AssignEngine(asIScriptEngine * engine)
{
	this->engine = engine;
}

rScriptFunction::rScriptFunction()
{
}

rScriptFunction::~rScriptFunction()
{
}

std::string rScriptFunction::GetScriptDeclaration()
{
	string result = "";
	result += this->returnType + " ";
	result += this->functionName + "(";
	for(int i = 0; i < this->args.size(); i++)
	{
		if (i != 0) result += ", ";
		result += this->args[i].type + " " + this->args[i].argName;
	}
	result += ")";
	return result;
}

rScriptFunctionArg_t ScrArg(std::string type, std::string argName, std::string argDesc)
{
	return rScriptFunctionArg_t{type, argName, argDesc};
}

std::vector<rScriptFunctionArg_t> ScrArgArray(int n, ...)
{
	std::vector<rScriptFunctionArg_t> r;
	va_list vl;
	va_start(vl, n);
	rScriptFunctionArg_t a;
	for (int i = 0; i < n; i++)
	{
		a = va_arg(vl, rScriptFunctionArg_t);
		r.push_back(a);
	}
	va_end(vl);
	return r;
}
