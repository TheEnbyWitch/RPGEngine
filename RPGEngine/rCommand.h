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

typedef struct CMD_S {
	const char * cmd;
	ConsoleCommand * func;
} CMD_T;

static CMD_T CMDs[]{
	{"echo", &CMD_Print},
	{"null", NULL}
};

class rCommand
{
public:
	rCommand();
	rCommand(const char* from);
	~rCommand();

	bool inputWasProcessed = false;
	void ProcessInput();

	bool Execute();

	std::string input;

	int tokenCount;
	std::string tokens[MAX_STRING_TOKENS];
};
