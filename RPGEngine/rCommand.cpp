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

	for (int i = 0; i < sizeof(CMDs) / sizeof(CMD_T); i++)
	{
		if (strcmp(CMDs[i].cmd, this->tokens[0].c_str()) == 0)
		{
			CMDs[i].func(a);
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
