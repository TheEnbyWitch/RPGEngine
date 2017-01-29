#pragma once
#include "stdafx.h"
void SCR_Print(std::string txt);

class rScript
{
public:
	rScript();
	~rScript();

	char* ReadScript(char* name);
	void ExecuteScript();

};

