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
	destinationCameraOffset.X = PositionX + (960.0 / -2.0) + 16.0;
	destinationCameraOffset.Y = PositionY + (540.0 / -2.0) + 16.0;
	rEntity::Frame();
}

bool rPlayer::Move(int x, int y)
{
	if (rEntity::Move(x, y))
	{
		//destinationCameraOffset.X += x * 32;
		//destinationCameraOffset.Y += y * 32;
		return true;
	}
	return false;
}
