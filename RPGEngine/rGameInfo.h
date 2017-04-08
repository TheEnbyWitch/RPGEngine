#pragma once

typedef struct rGMenuItem_s {
	ALLEGRO_MENU_INFO MI;
	char scriptFunction[64];
} rGMenuItem_t;

class rGameInfo
{
public:
	rGameInfo();
	~rGameInfo();

	void Init();
	ALLEGRO_MENU_INFO * GetMenuInfo();
	char * GetScriptFuncForItem(int id);

	int aboutID = 0;
	char gameName[64];
	char gameCompany[64];

private:

	std::vector<rGMenuItem_t> rawMenuInfo;
	ALLEGRO_MENU_INFO *rawMenuInfoArray = NULL;
};

