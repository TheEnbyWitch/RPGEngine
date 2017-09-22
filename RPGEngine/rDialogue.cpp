#include "stdafx.h"
#include "rDialogue.h"


rDialogue::rDialogue()
{
}


rDialogue::~rDialogue()
{
}

void rDialogue::Init()
{
}

void rDialogue::DoDialogue(const char* file)
{
	gameState = GAME_STATE_DIALOGUE;
	vm.RunDLGFile(file);
}

void rDialogue::Frame()
{
	timer++;
	while (!isWaitActive && vm.isActive)
	{
		vm.ExecuteNextInstruction();
	}
	if (nextWait <= timer)
	{
		isWaitActive = false;
	}
	if (currentWrittenIndex < content.length())
	{
		contentWritten.push_back(content[currentWrittenIndex++]);
	}
}

void rDialogue::Draw()
{
	gUI.DrawColoredWindowWithText(speaker.c_str(), (__width / 2) - (al_get_text_width(font, speaker.c_str()) / 2) -5, __height - (13 * 6) - 16, al_get_text_width(font, speaker.c_str()) + 10, 13, al_map_rgb(255,255,0), ALLEGRO_ALIGN_CENTER);
	gUI.DrawColoredWindowWithText((content).c_str(), 8, __height - (13 * 5) - 8, __width - (8 * 2), (13 * 5), al_map_rgb(255, 255, 255));
}

void rDialogue::Key(int keycode)
{
	if (keycode == ALLEGRO_KEY_ENTER && !vm.isActive)
	{
		active = false;
		gameState = GAME_STATE_INGAME;
	}
}

void rDialogue::SetID(const char * id)
{
	strcpy(this->dialogueID, id);
}

void rDialogue::SetSpeaker(const char * speaker)
{
	this->speaker = speaker;
}

void rDialogue::SetContent(const char * content)
{
	this->content = content;
	this->contentWritten = "";
	this->currentWrittenIndex = 0;
}

void rDialogue::PlayVOX(const char * vox)
{
	voxIntensity = gSound.PlayVoiceover(vox);
}

void rDialogue::SetLeftImage(bool show, bool animate, const char * img)
{
	strcpy(pathToLeftImg, img);
	showLeftImg = show;
	animateLeft = animate;
}

void rDialogue::SetRightImage(bool show, bool animate, const char * img)
{
	strcpy(pathToRightImg, img);
	showRightImg = show;
	animateRight = animate;
}

rDialogueFunction::rDialogueFunction(char opcode, const char * funcName, rDialogueFuncPTR * func, int args)
{
	this->opcode = opcode;
	this->funcName = funcName;
	this->func = func;
	this->args = args;
}

void rDialogueFunction::Execute(rDialogueInstruction p)
{
	this->func(p);
}

bool rDialogueFunction::Compare(const char * func)
{
	return strcmp(funcName.c_str(), func) == 0;
}

void DLG_SetSpeaker(rDialogueInstruction p)
{
	gDialogue.SetSpeaker(p.GetStack(1).c_str());
}

void DLG_SetContent(rDialogueInstruction p)
{
	gDialogue.SetContent(p.GetStack(1).c_str());
}

void DLG_PlayVOX(rDialogueInstruction p)
{
	gDialogue.PlayVOX(p.GetStack(1).c_str());
}

void DLG_SetLeftImage(rDialogueInstruction p)
{
	if (p.GetStack(1) == "true")
		gDialogue.showLeftImg = true;
	if (p.GetStack(1) == "false")
		gDialogue.showLeftImg = false;

	if (p.GetStackSize() < 3) return;
	if (p.GetStack(2) == "true")
		gDialogue.animateLeft = true;
	if (p.GetStack(2) == "false")
		gDialogue.animateLeft = false;

	if (p.GetStackSize() < 4) return;
	strcpy(gDialogue.pathToLeftImg, p.GetStack(3).c_str());
}

void DLG_SetRightImage(rDialogueInstruction p)
{
	if (p.GetStack(1) == "true")
		gDialogue.showRightImg = true;
	if (p.GetStack(1) == "false")
		gDialogue.showRightImg = false;

	if (p.GetStackSize() < 3) return;
	if (p.GetStack(2) == "true")
		gDialogue.animateRight = true;
	if (p.GetStack(2) == "false")
		gDialogue.animateRight = false;

	if (p.GetStackSize() < 4) return;
	strcpy(gDialogue.pathToRightImg, p.GetStack(3).c_str());
}

void DLG_Wait(rDialogueInstruction p)
{
	gDialogue.isWaitActive = true;
	gDialogue.nextWait = gDialogue.timer + atoi(p.GetStack(1).c_str());
}

void DLG_NULL(rDialogueInstruction p)
{
	if(p.opcode != DLG_TKN_NULL) rpge_printf("[rDialogue] DLG Function with opcode %X not implemented.\n", p.opcode);
}

string rDialogueInstruction::GetStack(int n)
{
	if (stackSize <= n) return "";
	return stack[n];
}

int rDialogueInstruction::GetStackSize()
{
	return stackSize;
}

void rDialogueVirtualMachine::RunDLGFile(const char * file)
{
	instructions.clear();
	ALLEGRO_FILE * dlgScript;
	dlgScript = al_fopen(file, "rb");
	if (!dlgScript)
	{
		rpge_printf("[Dialogue] Failed to load dialogue file %s\n", file);
		return;
	}
	int size = al_fsize(dlgScript);
	char line[2048];
	currentFile = file;
	currentLine = 1;
	int currentIndex = 0;
	while (!al_feof(dlgScript))
	{
		char currentchar;
		al_fread(dlgScript, &currentchar, 1);
		if (currentchar == '\r') continue;
		if (currentchar != '\n')
		{
			line[currentIndex++] = currentchar;
		}
		else
		{
			line[currentIndex++] = '\0';
			currentIndex = 0;
			instructions.push_back(ProcessLine(line));
			currentLine++;
		}
		if (al_feof(dlgScript))
		{
			line[currentIndex++] = '\0';
			instructions.push_back(ProcessLine(line));
		}
	}
	isActive = true;
	currentLine = 0;
}

rDialogueInstruction rDialogueVirtualMachine::ProcessLine(const char * line)
{
	rDialogueInstruction a;
	char t[8192];
	int len = strlen(line);
	strcpy(t, line);
	if (len <= 0) return a;
	bool isInStringLiteral = false;
	int currentTokenID = 0;

	for (int i = 0; i < len; i++)
	{
		if (i != 0)
		{
			if (t[i] == '"' && t[i - 1] != '\\') isInStringLiteral = !isInStringLiteral;
		}
		if (t[i] == ' ' && a.stack[currentTokenID].length() != 0)
		{
			if (!isInStringLiteral)
			{
				a.stack[currentTokenID] += '\0';
				currentTokenID++;
				continue;
			}
		}
		if (t[i] == '"' && t[i - 1] == '\\')
		{
			a.stack[currentTokenID] += t[i];
			continue;
		}
		if (t[i] == 'n' && t[i - 1] == '\\')
		{
			a.stack[currentTokenID] += '\n';
			continue;
		}

		if (t[i] == '\\') continue;
		if ((t[i] != ' ' || isInStringLiteral) && t[i] != '"') a.stack[currentTokenID] += t[i];
	}
	a.stackSize = currentTokenID + 1;
	a.line = currentLine;
	a.file = currentFile;
	if (a.GetStack(0)[a.GetStack(0).length()-1] == ':')
	{
		a.opcode = DLG_TKN_LABEL;
		rDialogueLabel l;
		l.label = a.GetStack(0);
		l.label.pop_back();
		l.line = currentLine;
		labels.push_back(l);
	}
	else {
		for (int i = 0; i < sizeof(DialogueFunctions) / sizeof(rDialogueFunction); i++)
		{
			if (DialogueFunctions[i].Compare(a.GetStack(0).c_str()))
			{
				a.opcode = DialogueFunctions[i].opcode;
			}
		}
	}
	return a;
}

void rDialogueVirtualMachine::ExecuteNextInstruction()
{
	if (currentLine >= instructions.size())
	{
		isActive = false;
		return;
	}
	if (instructions[currentLine].opcode == DLG_TKN_GOTO)
	{
		string lbl = instructions[currentLine].GetStack(1);
		for (auto l : labels)
		{
			if (l.label == lbl)
			{
				currentLine = l.line;
				if (currentLine >= instructions.size())
					isActive = false;
				return;
			}
		}
	}
	if (instructions[currentLine].opcode == DLG_TKN_JMP)
	{
		if(instructions[currentLine].GetStackSize() > 1)
			currentLine += atoi(instructions[currentLine].GetStack(1).c_str());
		else
			currentLine++;
		if (currentLine >= instructions.size())
			isActive = false;
		return;
	}
	for (int i = 0; i < sizeof(DialogueFunctions) / sizeof(rDialogueFunction); i++)
	{
		if (DialogueFunctions[i].Compare(instructions[currentLine].GetStack(0).c_str()))
		{
			DialogueFunctions[i].Execute(instructions[currentLine]);
			currentLine++;
			if (currentLine >= instructions.size())
			{
				isActive = false;
			}
			return;
		}
	}
}

void rDialogueVirtualMachine::JumpToLine(int line)
{
}
