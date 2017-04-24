#pragma once
#define DIALOGUE_STACK_SIZE 8 //includes the instruction itself

typedef enum {
	DLG_TKN_SET_SPEAKER		 = 0x00,
	DLG_TKN_SET_CONTENT		 = 0x01,
	DLG_TKN_PLAY_VOX		 = 0x02,
	DLG_TKN_SET_L_CHR_IMG	 = 0x03,
	DLG_TKN_SET_R_CHR_IMG	 = 0x04,
	DLG_TKN_SET_ACTIVE		 = 0x05,
	DLG_TKN_WAIT			 = 0x06,
	DLG_TKN_WAITTILL_VOX	 = 0x07,
	DLG_TKN_WAITTILL_KEY	 = 0x08,
	DLG_TKN_WAITTILL_VOXKEY	 = 0x09,
	DLG_TKN_JMPIF			 = 0x0a,
	DLG_TKN_GOTO			 = 0x0b,
	DLG_TKN_JMP				 = 0x0c,
	DLG_TKN_SET_VAR			 = 0x0d,
	DLG_TKN_SETCHOICECOUNT	 = 0x0e,
	DLG_TKN_CHOICE			 = 0x0f,
	DLG_TKN_EXEC_CHOICE		 = 0x10,

	DLG_TKN_LABEL,
	DLG_TKN_NULL
} rDialogueOpcodes_e;

class rDialogueInstruction
{
public:
	string GetStack(int n);
	int GetStackSize();

	char opcode = 0;
	int line;
	string file;

	string stack[DIALOGUE_STACK_SIZE];
	int stackSize;
};

typedef void (rDialogueFuncPTR)(rDialogueInstruction p);

class rDialogueFunction
{
public:
	rDialogueFunction(char opcode, const char* funcName, rDialogueFuncPTR * func, int args);
	void Execute(rDialogueInstruction p);
	bool Compare(const char * func);
	char opcode = 0;
	string funcName = "null";
	rDialogueFuncPTR * func = 0;
	int args = 0;
};

class rDialogueLabel
{
public:
	string label;
	int line;
};

class rDialogueVirtualMachine
{
public:
	void RunDLGFile(const char * file);
	rDialogueInstruction ProcessLine(const char * line);
	void ExecuteNextInstruction();

	bool isActive = false;
	int currentLine = 0;
	string currentFile = "";
	std::vector<rDialogueInstruction> instructions;
	std::vector<rDialogueLabel> labels;

private:
	void JumpToLine(int line);

};

class rDialogue
{
public:
	rDialogue();
	~rDialogue();

	void Init();
	void DoDialogue(const char* file);
	void Frame();
	void Draw();
	void Key(int keycode);

	void SetID(const char * id);

	void SetSpeaker(const char * speaker);
	void SetContent(const char * content);

	void PlayVOX(const char * vox);

	void SetLeftImage (bool show, bool animate, const char * img = "null");
	void SetRightImage(bool show, bool animate, const char * img = "null");

	char pathToLeftImg[256];
	char pathToRightImg[256];
	
	char pathToVOX[256];

	char dialogueID[64];

	bool showLeftImg = false;
	bool showRightImg = false;

	bool animateLeft = false;
	bool animateRight = false;

	bool active = false;

	float * voxIntensity = 0;

	string content;
	string speaker;

	rDialogueVirtualMachine vm;
	int timer = 0;
	int nextWait = 0;
	bool isWaitActive = false;
};

// DLG FILES
void DLG_SetSpeaker(rDialogueInstruction p);
void DLG_SetContent(rDialogueInstruction p);

void DLG_PlayVOX(rDialogueInstruction p);

void DLG_SetLeftImage(rDialogueInstruction p);
void DLG_SetRightImage(rDialogueInstruction p);

//UTILS
void DLG_Wait(rDialogueInstruction p);

void DLG_NULL(rDialogueInstruction p);

static rDialogueFunction DialogueFunctions[]{
	{ DLG_TKN_SET_SPEAKER, "speaker", &DLG_SetSpeaker, 1 },
	{ DLG_TKN_SET_CONTENT, "content", &DLG_SetContent, 1 },
	{ DLG_TKN_PLAY_VOX, "playvox", &DLG_PlayVOX, 1},
	{ DLG_TKN_SET_L_CHR_IMG, "lchrimg", &DLG_SetLeftImage, 1},
	{ DLG_TKN_SET_R_CHR_IMG, "rchrimg", &DLG_SetRightImage, 1},
	{ DLG_TKN_SET_ACTIVE, "setactive", &DLG_NULL, 1},
	{ DLG_TKN_WAIT, "wait", &DLG_Wait, 1},
	{ DLG_TKN_GOTO, "goto", &DLG_NULL, 1} // handled by VM
};