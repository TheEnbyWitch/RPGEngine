#pragma once

enum rDvarType_e {
	DVAR_BOOL,
	DVAR_INTEGER,
	DVAR_FLOAT,
	DVAR_STRING
};

// Developer Var
class rDVar
{
public:
	rDVar();
	rDVar(std::string name, rDvarType_e type);
	rDVar(const char * name, rDvarType_e type);
	rDVar(char * name, rDvarType_e type);
	~rDVar();

	const char * GetName();
	rDvarType_e GetType();
	const char * GetTypeConstChar();

	const std::string& GetStdStringDescription();
	const char* GetConstCharDescription();

	bool			ToBool();
	int				ToInt();
	float			ToFloat();
	std::string		ToStdString();
	const char*		ToConstChar();

	void SetValue(int val);
	void SetValue(float val);
	void SetValue(const std::string& val);
	void SetValue(char * val);
	void SetValue(const char * val);

	void SetDescription(const std::string& val);
	void SetDescription(const char * val);
	void SetDescription(char * val);

private:

	rDvarType_e type;
	bool valueBOOL = false;
	int valueINT = 0;
	float valueFLOAT = 0.0f;
	std::string valueSTRING = "";
	std::string name = "";
	std::string description = "";
};