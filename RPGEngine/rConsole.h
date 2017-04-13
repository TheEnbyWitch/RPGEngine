#pragma once
class rConsole
{
public:
	rConsole();
	~rConsole();

	int currentCOffset = 0;

	void Draw();
	void Key(int keycode);

	rDVar * GetDvar(char* name);
	rDVar * GetDvar(const char* name);
	rDVar * GetDvar(const std::string& name);

	rDVar * RegisterDVar(char* name, rDvarType_e type);
	rDVar * RegisterDVar(const char* name, rDvarType_e type);
	rDVar * RegisterDVar(const std::string& name, rDvarType_e type);



#define RG_DVP(t,t2) \
	rDVar * RegisterDVar(t name, rDvarType_e type, t2 defaultVal);

#define RG_DV(t,t2) \
	rDVar * rConsole::RegisterDVar(t name, rDvarType_e type, t2 defaultVal) \
	{ \
		rDVar * newDVAR = new rDVar(name, type); \
		newDVAR->SetValue(defaultVal); \
		DVars.push_back(newDVAR); \
		return newDVAR; \
	} 

	RG_DVP(char *, int);
	RG_DVP(char *, float);
	RG_DVP(char *, char*);
	RG_DVP(char *, const char*);
	RG_DVP(char *, const std::string&);
	/*
	RG_DVP(const char *, int);
	RG_DVP(const char *, float);
	RG_DVP(const char *, char*);
	RG_DVP(const char *, const char*);
	RG_DVP(const char *, const std::string&);
	*/
	RG_DVP(const std::string&, int);
	RG_DVP(const std::string&, float);
	RG_DVP(const std::string&, char*);
	RG_DVP(const std::string&, const char*);
	RG_DVP(const std::string&, const std::string&);

	std::vector<rDVar *> DVars;
};

