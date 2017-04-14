#pragma once
#define MAX_STRING_TOKENS 32
#include "Common.h"

class rCommandArgs
{
public:
	int tokenCount;
	std::string tokens[MAX_STRING_TOKENS];
	const char* GetArg(int at);
};

typedef void ConsoleCommand(rCommandArgs args);
void CMD_Print(rCommandArgs args);

void CMD_Set(rCommandArgs args);
void CMD_Seta(rCommandArgs args);

void CMD_SetMusicState(rCommandArgs args);

typedef struct CMD_S {
	const char * cmd;
	ConsoleCommand * func;
	const char * desc;
} CMD_T;
namespace rConsoleCMD
{
	static CMD_T CMDs[]{
		{ "set",			&CMD_Set,				"Set a DVar's value\n\nset <dvar> [value]"},
		{ "seta",			&CMD_Seta,				"Set a DVar's value and save it in the config file\n\nseta <dvar> [value]"},
		{ "echo",			&CMD_Print,				"Used for testing arguments\n\necho [args]"},
		{ "setmusicstate",	&CMD_SetMusicState,		"Set music state\n\nsetmusicstate <state>"}
	};
}
class rCommand
{
public:
	rCommand();
	rCommand(const char* from);
	~rCommand();

	bool inputWasProcessed = false;
	void ProcessInput();

	static rCommandArgs GetArgs(const char* from);

	bool Execute();

	std::string input;

	int tokenCount;
	std::string tokens[MAX_STRING_TOKENS];
};
