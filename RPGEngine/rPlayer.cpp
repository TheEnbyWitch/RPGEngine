#include "stdafx.h"
#include "rPlayer.h"


rPlayer::rPlayer()
{
	
}


rPlayer::~rPlayer()
{

}

void rPlayer::Frame()
{
	// ((currenttick / endtick) X current_value / end_result
	// Thanks portie
	rMath::SpringDamp(&cameraOffset, destinationCameraOffset, 1.75f);

	rEntity::Frame();
}

bool rPlayer::Move(int x, int y)
{
	if (rEntity::Move(x, y))
	{
		destinationCameraOffset.X += x * 32;
		destinationCameraOffset.Y += y * 32;
		return true;
	}
	return false;
}
