#include "stdafx.h"
#include "rConsole.h"

rConsole gConsole;

rConsole::rConsole()
{
}


rConsole::~rConsole()
{
}

void rConsole::Draw()
{
	if (showCon)
	{
		std::string resultConLog;
		int currentLines = 0;
		int lines = 0;
		for (int i = 0; i < consoleLog.length(); i++)
		{
			if (consoleLog[i] == '\n')
				lines++;
		}
		if (lineOffset + MAX_LINES_SHOWN > lines) lineOffset = lines - MAX_LINES_SHOWN;
		for (int i = consoleLog.length() - 2; i >= 0; i--)
		{
			if (consoleLog[i] == '\n')
			{
				currentLines++;
			}
			if (currentLines > MAX_LINES_SHOWN + lineOffset)
				break;
			if (currentLines < lineOffset) continue;
			resultConLog.insert(0, 1, consoleLog[i]);
		}

		std::string resultConInput = consoleInput;
		//(frames % 40 > 20 ? "_" : "")
		if (resultConInput.length() != currentCOffset)
			resultConInput[currentCOffset] = (frames % 40 > 20 ? '_' : resultConInput[currentCOffset]);
		else
			resultConInput[currentCOffset] += (frames % 40 > 20 ? '_' : '\0');

		gUI.DrawColoredWindowWithText(resultConLog.c_str(), 6, 6, gRenderer.displayWidth - (6 * 2), (MAX_LINES_SHOWN + 1) * 13, al_map_rgb(0, 128, 255));
		gUI.DrawColoredWindowWithText(va("%s%s", ENGINE_STR, resultConInput.c_str()), 6, ((MAX_LINES_SHOWN + 1) * 13) + 12, gRenderer.displayWidth - (6 * 2), 14, al_map_rgb(255, 255, 0), ALLEGRO_ALIGN_LEFT);
		
		// x: 6 + al_get_text_width(font, ENGINE_STR)
		// y: ((MAX_LINES_SHOWN + 1) * 13) + 26
		if (consoleInput.length() == 0) return;
		std::string autocomplete = "";
		int completions = 0;
		int maxWidth = 0;
		int index = 0;
		std::string cmp;
		std::string firstToken = consoleInput.substr(0, consoleInput.find_first_of(' ') == std::string::npos ? consoleInput.length()-1 : consoleInput.find_first_of(' '));
		rCommandArgs cmd = rCommand::GetArgs(consoleInput.c_str());
		if (cmd.tokenCount > 0)
		{
			if (strcmp(cmd.GetArg(0), "set") != 0 && strcmp(cmd.GetArg(0), "seta") != 0)
			{
				for (int i = 0; i < sizeof(rConsoleCMD::CMDs) / sizeof(CMD_T); i++)
				{
					/*
					std::string s = "one two three";
		auto first_token = s.substr(0, s.find(' '));
		*/
					if (cmd.tokenCount < 2)
					{
						if (strncmp(cmd.GetArg(0), rConsoleCMD::CMDs[i].cmd, consoleInput.length()) == 0)
						{
							index = i;
							completions++;
							std::string result = rConsoleCMD::CMDs[i].cmd;
							if (rConsoleCMD::CMDs[i].desc != NULL)
								cmp = std::string(rConsoleCMD::CMDs[i].desc);
							autocomplete += result;
							maxWidth = __max(maxWidth, al_get_text_width(font, result.c_str()) + 10);
							autocomplete += "\n";
						}
					}
					else {
						if (strcmp(cmd.GetArg(0), rConsoleCMD::CMDs[i].cmd) == 0)
						{
							index = i;
							completions++;
							std::string result = rConsoleCMD::CMDs[i].cmd;
							if (rConsoleCMD::CMDs[i].desc != NULL)
								cmp = std::string(rConsoleCMD::CMDs[i].desc);
							autocomplete += result;
							maxWidth = __max(maxWidth, al_get_text_width(font, result.c_str()) + 10);
							autocomplete += "\n";
						}
					}
				}
			}
			else {
				for(int i = 0; i < DVars.size(); i++)
				{
					rDVar * dvar = DVars[i];
					if (cmd.tokenCount < 3)
					{
						if (strncmp(cmd.GetArg(1), dvar->GetName(), strlen(cmd.GetArg(1))) == 0)
						{
							index = i;
							completions++;
							std::string result = dvar->GetName();
							cmp = "Value: " + dvar->ToStdString();
							autocomplete += result;
							maxWidth = __max(maxWidth, al_get_text_width(font, result.c_str()) + 10);
							autocomplete += "\n";
						}
					}
					else {
						if (strcmp(cmd.GetArg(1), dvar->GetName()) == 0)
						{
							index = i;
							completions++;
							std::string result = dvar->GetName();
							cmp = "Value: " + dvar->ToStdString();
							autocomplete += result;
							maxWidth = __max(maxWidth, al_get_text_width(font, result.c_str()) + 10);
							autocomplete += "\n";
						}
					}
				}
			}
		}
		if(autocomplete.length()) autocomplete.pop_back();
		if (completions)
		{
			gUI.DrawColoredWindowWithText(autocomplete.c_str(), 6 + al_get_text_width(font, ENGINE_STR) + 3, ((MAX_LINES_SHOWN + 1) * 13) + 32, maxWidth, 13 * completions, al_map_rgb(255, 0, 0));
			if (completions == 1)
			{
				gUI.DrawColoredWindowWithText(cmp.c_str(), 6 + al_get_text_width(font, ENGINE_STR) + 3, ((MAX_LINES_SHOWN + 1) * 13) + 32 + 13 + 6, al_get_text_width(font, cmp.c_str()) + 10, 13 * completions, al_map_rgb(0, 255, 0));
			}
		}
	}
}

void rConsole::Key(int keycode)
{
	char input = GetCharFromKeycode(keycode, (isShiftPressed ? ALLEGRO_KEYMOD_SHIFT : 0));
	if (keycode == ALLEGRO_KEY_BACKSPACE)
		if (consoleInput.length() > 0)
		{
			consoleInput.erase(currentCOffset - 1, 1);
			currentCOffset--;
		}

	if (keycode == ALLEGRO_KEY_LEFT)
	{
		currentCOffset = __max(0, currentCOffset - 1);
	}
	if (keycode == ALLEGRO_KEY_RIGHT)
	{
		currentCOffset = __min(currentCOffset + 1, consoleInput.length());
	}

	if (input > 0)
	{
		consoleInput.insert(consoleInput.begin() + currentCOffset, input);
		currentCOffset++;
		//consoleInput += input;
	}
	if (keycode == ALLEGRO_KEY_ENTER)
	{
		rCommand cmd(consoleInput.c_str());
		cmd.ProcessInput();
		cmd.Execute();
		consoleInput.clear();
		currentCOffset = 0;
	}
}

rDVar * rConsole::GetDvar(char * name)
{
	for (auto dvar : DVars)
	{
		if (strcmp(dvar->GetName(), name) == 0)
			return dvar;
	}
	return nullptr;
}

rDVar * rConsole::GetDvar(const char * name)
{
	for (auto dvar : DVars)
	{
		if (strcmp(dvar->GetName(), name) == 0)
			return dvar;
	}
	return nullptr;
}

rDVar * rConsole::GetDvar(const std::string& name)
{
	for (auto dvar : DVars)
	{
		if (strcmp(dvar->GetName(), name.c_str()) == 0)
			return dvar;
	}
	return nullptr;
}

rDVar * rConsole::RegisterDVar(char * name, rDvarType_e type)
{
	rDVar * newDVAR = new rDVar(name, type);
	newDVAR->SetValue(0);
	DVars.push_back(newDVAR);
	return newDVAR;
}

rDVar * rConsole::RegisterDVar(const char * name, rDvarType_e type)
{
	rDVar * newDVAR = new rDVar(name, type);
	newDVAR->SetValue(0);
	DVars.push_back(newDVAR);
	return newDVAR;
}

rDVar * rConsole::RegisterDVar(const std::string& name, rDvarType_e type)
{
	rDVar * newDVAR = new rDVar(name, type);
	newDVAR->SetValue(0);
	DVars.push_back(newDVAR);
	return newDVAR;
}

RG_DV(char *, int);
RG_DV(char *, float);
RG_DV(char *, char*);
RG_DV(char *, const char*);
RG_DV(char *, const std::string&);
/*
RG_DV(const char *, int);
RG_DV(const char *, float);
RG_DV(const char *, char*);
RG_DV(const char *, const char*);
RG_DV(const char *, std::string);
*/
RG_DV(const std::string&, int);
RG_DV(const std::string&, float);
RG_DV(const std::string&, char*);
RG_DV(const std::string&, const char*);
RG_DV(const std::string&, const std::string&);

