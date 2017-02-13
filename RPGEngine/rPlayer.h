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
	float cameraOffsetX = 0;
	float cameraOffsetY = 0;
	float targetCameraOffsetX = 0;
	float targetCameraOffsetY = 0;
};

