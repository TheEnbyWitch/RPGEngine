#pragma once
#include "stdafx.h"

class rPlayer : public rEntity
{
public:
	rPlayer();
	~rPlayer();
	/*
	virtual int* GetScreenPos();
	virtual void SetImage(char * path);
	virtual void Draw();
	*/
	void Frame();

	bool Move(int x, int y);
public:
	rVector2 cameraOffset = rVector2((960.0 / -2.0)+16.0, (540.0 / -2.0)+16.0);
	rVector2 destinationCameraOffset = rVector2((960.0 / -2.0)+16.0, (540.0 / -2.0)+16.0);
};

