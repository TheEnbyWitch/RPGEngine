#include "stdafx.h"
#include "rDialogue.h"


rDialogue::rDialogue()
{
}


rDialogue::~rDialogue()
{
}

void rDialogue::Frame()
{
	timer++;
}

void rDialogue::Draw()
{
	gUI.DrawColoredWindowWithText(speaker.c_str(), (__width / 2) - (al_get_text_width(font, speaker.c_str()) / 2) -5, __height - (13 * 6) - 16, al_get_text_width(font, speaker.c_str()) + 10, 13, al_map_rgb(255,255,0), ALLEGRO_ALIGN_CENTER);
	gUI.DrawColoredWindowWithText((content).c_str(), 8, __height - (13 * 5) - 8, __width - (8 * 2), (13 * 5), al_map_rgb(255, 255, 255));
}

void rDialogue::Key(int keycode)
{
	if (keycode == ALLEGRO_KEY_ENTER)
	{
		active = false;
		gameState = GAME_STATE_INGAME;
	}
}

void rDialogue::SetID(char * id)
{
	strcpy(this->dialogueID, id);
}

void rDialogue::SetSpeaker(char * speaker)
{
	this->speaker = speaker;
}

void rDialogue::SetContent(char * content)
{
	this->content = content;
}

void rDialogue::SetVOX(char * vox)
{
	strcpy(pathToVOX, vox);
}

void rDialogue::SetLeftImage(bool show, bool animate, char * img)
{
	strcpy(pathToLeftImg, img);
	showLeftImg = show;
	animateLeft = animate;
}

void rDialogue::SetRightImage(bool show, bool animate, char * img)
{
	strcpy(pathToRightImg, img);
	showRightImg = show;
	animateRight = animate;
}
