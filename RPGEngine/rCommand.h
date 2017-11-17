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

void CMD_RunDialogue(rCommandArgs args);

void CMD_Quit(rCommandArgs args);

void CMD_Teleport(rCommandArgs args);
void CMD_Time(rCommandArgs args);

void CMD_VidRestart(rCommandArgs args);

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
		{ "setmusicstate",	&CMD_SetMusicState,		"Set music state\n\nsetmusicstate <state>"},
		{ "rundialogue",	&CMD_RunDialogue,		"Runs the specified dialogue file\n\nrundialogue <file>"},
		{ "quit",			&CMD_Quit,				"Quit the game" },
		{ "time",			&CMD_Time,				"Set or add time to Time of Day\n\ntime <add/set> 0-24000"},
		{ "teleport",       &CMD_Teleport,			"Teleport the player\n\nteleport <x> <y>" },
		{ "vid_restart",	&CMD_VidRestart,		"Restart display" },
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
