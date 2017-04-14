#include "stdafx.h"
#include "rDVar.h"


rDVar::rDVar()
{
}

rDVar::rDVar(std::string name, rDvarType_e type)
{
	this->name = name;
	this->type = type;
}

rDVar::rDVar(const char * name, rDvarType_e type)
{
	this->name = name;
	this->type = type;
}

rDVar::rDVar(char * name, rDvarType_e type)
{
	this->name = name;
	this->type = type;
}

rDVar::~rDVar()
{
}

const char * rDVar::GetName()
{
	return name.c_str();
}

rDvarType_e rDVar::GetType()
{
	return type;
}

const char * rDVar::GetTypeConstChar()
{
	switch (type)
	{
	case DVAR_BOOL:
		return "DVAR_BOOL";
	case DVAR_FLOAT:
		return "DVAR_FLOAT";
	case DVAR_INTEGER:
		return "DVAR_INTEGER";
	case DVAR_STRING:
		return "DVAR_STRING";
	}
	return "DVAR_UNKNOWN";
}

const std::string & rDVar::GetStdStringDescription()
{
	return description;
}

const char * rDVar::GetConstCharDescription()
{
	return description.c_str();
}

bool rDVar::ToBool()
{
	return valueBOOL;
}

int rDVar::ToInt()
{
	return valueINT;
}

float rDVar::ToFloat()
{
	return valueFLOAT;
}

std::string rDVar::ToStdString()
{
	return valueSTRING;
}

const char * rDVar::ToConstChar()
{
	return valueSTRING.c_str();
}

void rDVar::SetValue(int val)
{
	if (type == DVAR_BOOL) val = (val != 0 ? true : false);
	valueBOOL = val;
	valueINT = val;
	valueFLOAT = val;
	valueSTRING = std::to_string(val);
}

void rDVar::SetValue(float val)
{
	if (type == DVAR_BOOL) val = (val != 0 ? true : false);
	valueBOOL = val;
	valueINT = val;
	valueFLOAT = val;
	valueSTRING = std::to_string(val);
}

void rDVar::SetValue(const std::string& val)
{
	std::string localCopy = val;
	if (type == DVAR_BOOL)
	{
		std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
		if (localCopy == "true" || localCopy == "1")
		{
			valueBOOL = true;
			valueINT = 1;
			valueFLOAT = 1.0f;
			valueSTRING = "true";
		}
		else {
			valueBOOL = false;
			valueINT = 0;
			valueFLOAT = 0.0f;
			valueSTRING = "false";
		}
		return;
	}
	valueBOOL = (val == "true" ? true : false);
	valueINT = atoi(val.c_str());
	valueFLOAT = atof(val.c_str());
	valueSTRING = val;
}

void rDVar::SetValue(char * val)
{
	std::string localCopy = std::string(val);
	if (type == DVAR_BOOL)
	{
		std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
		if (localCopy == "true" || localCopy == "1")
		{
			valueBOOL = true;
			valueINT = 1;
			valueFLOAT = 1.0f;
			valueSTRING = "true";
		}
		else {
			valueBOOL = false;
			valueINT = 0;
			valueFLOAT = 0.0f;
			valueSTRING = "false";
		}
		return;
	}
	valueBOOL = (std::string(val) == "true" ? true : false);
	valueINT = atoi(val);
	valueFLOAT = atof(val);
	valueSTRING = std::string(val);
}

void rDVar::SetValue(const char * val)
{
	std::string localCopy = std::string(val);
	if (type == DVAR_BOOL)
	{
		std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
		if (localCopy == "true" || localCopy == "1")
		{
			valueBOOL = true;
			valueINT = 1;
			valueFLOAT = 1.0f;
			valueSTRING = "true";
		}
		else {
			valueBOOL = false;
			valueINT = 0;
			valueFLOAT = 0.0f;
			valueSTRING = "false";
		}
		return;
	}
	valueBOOL = (std::string(val) == "true" ? true : false);
	valueINT = atoi(val);
	valueFLOAT = atof(val);
	valueSTRING = std::string(val);
}

void rDVar::SetDescription(const std::string & val)
{
	description = val;
}

void rDVar::SetDescription(const char * val)
{
	description = std::string(val);
}

void rDVar::SetDescription(char * val)
{
	description = std::string(val);
}
