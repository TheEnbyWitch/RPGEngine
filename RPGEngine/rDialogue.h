#pragma once
class rDialogue
{
public:
	rDialogue();
	~rDialogue();

	void Frame();
	void Draw();
	void Key(int keycode);

	void SetID(char * id);

	void SetSpeaker(char * speaker);
	void SetContent(char * content);

	void SetVOX(char * vox);

	void SetLeftImage (bool show, bool animate, char * img = "null");
	void SetRightImage(bool show, bool animate, char * img = "null");

	char pathToLeftImg[256];
	char pathToRightImg[256];
	
	char pathToVOX[256];

	char dialogueID[64];

	bool showLeftImg = false;
	bool showRightImg = false;

	bool animateLeft = false;
	bool animateRight = false;

	bool active = false;

	string content;
	string speaker;

private:
	bool VOXPlayed = false;
	int timer = 0;
};

