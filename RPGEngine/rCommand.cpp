#include "stdafx.h"
#include "rCommand.h"


rCommand::rCommand()
{
}

rCommand::rCommand(const char * from)
{
	input = from;
}


rCommand::~rCommand()
{
}

void rCommand::ProcessInput()
{
	char t[8192];
	int len = input.length();
	strcpy(t, input.c_str());

	bool isInStringLiteral = false;
	int currentTokenID = 0;

	for (int i = 0; i < len; i++)
	{
		if (i != 0)
		{
			if (t[i] == '"' && t[i - 1] != '\\') isInStringLiteral = !isInStringLiteral;
		}
		if (t[i] == ' ' && tokens[currentTokenID].length() != 0)
		{
			if (!isInStringLiteral)
			{
				tokens[currentTokenID] += '\0';
				currentTokenID++;
				continue;
			}
		}
		if (t[i] == '"' && t[i - 1] == '\\')
		{
			tokens[currentTokenID] += t[i];
			continue;
		}
		if (t[i] == 'n' && t[i - 1] == '\\')
		{
			tokens[currentTokenID] += '\n';
			continue;
		}

		if (t[i] == '\\') continue;
		if ((t[i] != ' ' || isInStringLiteral) && t[i] != '"') tokens[currentTokenID] += t[i];
	}
	tokenCount = currentTokenID + 1;
	inputWasProcessed = true;
}

rCommandArgs rCommand::GetArgs(const char* from)
{
	rCommandArgs a;
	char t[8192];
	int len = strlen(from);
	strcpy(t, from);

	bool isInStringLiteral = false;
	int currentTokenID = 0;

	for (int i = 0; i < len; i++)
	{
		if (i != 0)
		{
			if (t[i] == '"' && t[i - 1] != '\\') isInStringLiteral = !isInStringLiteral;
		}
		if (t[i] == ' ' && a.tokens[currentTokenID].length() != 0)
		{
			if (!isInStringLiteral)
			{
				a.tokens[currentTokenID] += '\0';
				currentTokenID++;
				continue;
			}
		}
		if (t[i] == '"' && t[i - 1] == '\\')
		{
			a.tokens[currentTokenID] += t[i];
			continue;
		}
		if (t[i] == 'n' && t[i - 1] == '\\')
		{
			a.tokens[currentTokenID] += '\n';
			continue;
		}

		if (t[i] == '\\') continue;
		if ((t[i] != ' ' || isInStringLiteral) && t[i] != '"') a.tokens[currentTokenID] += t[i];
	}
	a.tokenCount = currentTokenID + 1;
	return a;
}

bool rCommand::Execute()
{
	if (!inputWasProcessed)
	{
		rpge_printf("[rCommand] Cannot execute command! Input was not processed!\n");
		return false;
	}
	rCommandArgs a;
	a.tokenCount = tokenCount;
	for(int i = 0; i < tokenCount; i++)
	{
		a.tokens[i] = tokens[i];
	}
	std::string localCopy = this->tokens[0];
	std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
	for (int i = 0; i < sizeof(rConsoleCMD::CMDs) / sizeof(CMD_T); i++)
	{
		if (strcmp(rConsoleCMD::CMDs[i].cmd, this->tokens[0].c_str()) == 0)
		{
			if (rConsoleCMD::CMDs[i].func != NULL)
				rConsoleCMD::CMDs[i].func(a);
			else
				abort_game(va("\"%s\" command doesn't have a function assigned.\nPlease check the rCommand.h file!", rConsoleCMD::CMDs[i].cmd));
			return true;
		}
	}
	return true;
}

const char * rCommandArgs::GetArg(int at)
{
	return tokens[at].c_str();
}

void CMD_Print(rCommandArgs args)
{
	rpge_printf("echo: Arguments: %d\n", args.tokenCount);
	for (int i = 0; i < args.tokenCount; i++)
	{
		rpge_printf("Arg %d: %s\n", i, args.GetArg(i));
	}
}

void CMD_Set(rCommandArgs args)
{
	if(args.tokenCount > 1)
	{
		if (gConsole.GetDvar(args.GetArg(1)) != NULL)
		{
			if (args.tokenCount > 2)
				gConsole.GetDvar(args.GetArg(1))->SetValue(args.GetArg(2));
			else
				rpge_printf("%s is \"%s\"\n", args.GetArg(1), gConsole.GetDvar(args.GetArg(1))->ToConstChar());
		}
		else {
			if (args.tokenCount > 2)
				gConsole.RegisterDVar(args.GetArg(1), DVAR_STRING, args.GetArg(2));
			else
				rpge_printf("%s is not a valid dvar\n", args.GetArg(1));
		}
	}
	else {
		rpge_printf("Usage: set <dvar> [value]\n");
	}
}

void CMD_Seta(rCommandArgs args)
{
	rpge_printf("[CMD_Seta] Not yet implemented, calling CMD_Set\n");
	CMD_Set(args);
}

void CMD_SetMusicState(rCommandArgs args)
{
	if(args.tokenCount >= 2)
		gSound.PlayMusic(args.GetArg(1));
}

void CMD_RunDialogue(rCommandArgs args)
{
	gDialogue.DoDialogue(args.GetArg(1));
}
