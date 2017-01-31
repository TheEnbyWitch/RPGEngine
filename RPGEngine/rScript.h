#pragma once
#include "stdafx.h"

#define SCR_FUNC(x) int SCR_##x(lua_State *state)

class rScript
{
public:
	rScript();
	~rScript();

	lua_State *luaState;
	int lastResult;

	char* ReadScript(char* name);
	void ExecuteScript();

};

SCR_FUNC(Print);