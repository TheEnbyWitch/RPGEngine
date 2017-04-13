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
	valueBOOL = val;
	valueINT = val;
	valueFLOAT = val;
	valueSTRING = std::to_string(val);
}

void rDVar::SetValue(float val)
{
	valueBOOL = val;
	valueINT = val;
	valueFLOAT = val;
	valueSTRING = std::to_string(val);
}

void rDVar::SetValue(const std::string& val)
{
	valueBOOL = (val == "true" ? true : false);
	valueINT = atoi(val.c_str());
	valueFLOAT = atof(val.c_str());
	valueSTRING = val;
}

void rDVar::SetValue(char * val)
{
	valueBOOL = (std::string(val) == "true" ? true : false);
	valueINT = atoi(val);
	valueFLOAT = atof(val);
	valueSTRING = std::string(val);
}

void rDVar::SetValue(const char * val)
{
	valueBOOL = (std::string(val) == "true" ? true : false);
	valueINT = atoi(val);
	valueFLOAT = atof(val);
	valueSTRING = std::string(val);
}
