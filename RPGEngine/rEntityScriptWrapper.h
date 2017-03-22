#ifndef __RPG_RENTITYSCRIPTWRAPPER
#define __RPG_RENTITYSCRIPTWRAPPER

class rEntityScriptWrapper
{
public:
	rEntityScriptWrapper();
	~rEntityScriptWrapper();

	void SetEntityPointer(rEntity *pointer);
	void GetEnt(char * id);
	void Spawn(char * id);

	bool HasAnAssignedEnt();

	bool Move(int x, int y);
	void ChangeDirection(int targetDirection);
	void SetImage(char* path);

	int GetIntValue(char * key);
	bool GetBoolValue(char * key);
	void SetIntValue(char * key, int value);
	void SetBoolValue(char * key, bool value);
private:
	rEntity* target;
	bool hasAssignedEnt = false;
};

#endif