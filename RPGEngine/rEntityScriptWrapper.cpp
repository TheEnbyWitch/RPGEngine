#include "stdafx.h"
#include "rEntityScriptWrapper.h"


#define ONLY_IF_HAS_ASSIGNED_ENT	\
{									\
	if(!hasAssignedEnt)				\
		abort_game("rEntityScriptWrapper doesn't have an ent assigned!"); \
	if(target == NULL) \
		abort_game("rEntityScriptWrapper's target is NULL!"); \
}

rEntityScriptWrapper::rEntityScriptWrapper()
{
	target = NULL;
	hasAssignedEnt = false;
}


rEntityScriptWrapper::~rEntityScriptWrapper()
{
}

rEntityScriptWrapper * rEntityScriptWrapper::operator=(rEntityScriptWrapper &rhs) const
{
	rEntityScriptWrapper *n = new rEntityScriptWrapper();
	memcpy(n, &rhs, sizeof(rEntityScriptWrapper));
	return n;
}

void rEntityScriptWrapper::SetEntityPointer(rEntity * pointer)
{
	target = pointer;
	hasAssignedEnt = true;
}

rEntity * rEntityScriptWrapper::GetEntityPointer()
{
	return target;
}

void rEntityScriptWrapper::GetEnt(char * id)
{
	for (auto e : entityList)
	{
		if (strcmp(id, e->uniqueID) == 0)
		{
			SetEntityPointer(e);
			return;
		}
	}
}

void rEntityScriptWrapper::Spawn(char * id)
{
	rEntity * newEnt = rEntity::SpawnEntity();
	newEnt->Activate(id, true);
	SetEntityPointer(newEnt);
}

bool rEntityScriptWrapper::HasAnAssignedEnt()
{
	return hasAssignedEnt;
}

bool rEntityScriptWrapper::Move(int x, int y)
{
	ONLY_IF_HAS_ASSIGNED_ENT;
	return target->Move(x, y);
}

void rEntityScriptWrapper::ChangeDirection(int targetDirection)
{
	ONLY_IF_HAS_ASSIGNED_ENT;
	target->ChangeDirection(targetDirection);
}

void rEntityScriptWrapper::SetImage(char * path)
{
	ONLY_IF_HAS_ASSIGNED_ENT;
	target->SetImage(path);
}

#define GetValue(str, val) if(strcmp(key, str) == 0) { return val; }

int rEntityScriptWrapper::GetIntValue(char * key)
{
	GetValue("layer", target->Layer);
	GetValue("level", target->Level);
	GetValue("speed", target->Speed);
	rpge_printf("[rEntity] Failed to get a value for (int)%s\n", key);
	return 0;
}

bool rEntityScriptWrapper::GetBoolValue(char * key)
{
	GetValue("useEmissive", target->useEmissive);
	rpge_printf("[rEntity] Failed to get a value for (bool)%s\n", key);
	return false;
}
#define SetValueTo(str, val) if(strcmp(key, str) == 0) { val = value; return;	}

void rEntityScriptWrapper::SetIntValue(char * key, int value)
{
	SetValueTo("layer", target->Layer);
	SetValueTo("level", target->Level);
	SetValueTo("speed", target->Speed);
	rpge_printf("[rEntity] Failed to set a value of \"%d\" for (int)%s\n", value, key);
}

void rEntityScriptWrapper::SetBoolValue(char * key, bool value)
{
	SetValueTo("useEmissive", target->useEmissive);
	rpge_printf("[rEntity] Failed to set a value of \"%s\" for (bool)%s\n", (value ? "true" : "false"), key);
}
