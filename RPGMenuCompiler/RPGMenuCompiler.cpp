// RPGMenuCompiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include "../RPGEngine/Menu.h"
#include "../RPGEngine/json.hpp"

using namespace std;
using json = nlohmann::json;
rMenu::rMenu() {}
rMenu::~rMenu() {}

vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void PrintUsage()
{
	cout << "*************" << endl
		 << "* MENUCMPLR *" << endl
		 << "*************" << endl;
}

void Process(string input, string name)
{
	try
	{
		json inputFile;
		ifstream inputFileStream;
		cout << "Processing " << name << endl;
		inputFileStream.open(input);
		inputFileStream >> inputFile;
		rMenu result;
		string menuName = inputFile["name"].get<string>();
		int items = inputFile["items"].size();
		cout << "Menu name:  " << inputFile["name"].get<string>() << endl;
		cout << "Item count: " << inputFile["items"].size() << endl;
		result.name = (char *)menuName.c_str();
		for (int i = 0; i < items; i++)
		{
			for (json::iterator it = inputFile["items"][i].begin(); it != inputFile["items"][i].end(); ++it) {
				rMenuItem_t item;
				if (it.key() == "rect")
				{
					item.rect = rRect_t{ it.value()[0].get<int>(),it.value()[1].get<int>(),it.value()[2].get<int>(),it.value()[3].get<int>() };
				}
				if (it.key() == "type")
				{
					item.type = (rMenuItemType)((int)it.value());
				}
				if (it.key() == "index")
				{
					item.buttonAttributes.index = it.value();
				}
				if (it.key() == "text")
				{
					item.text = (char *)it.value().get<string>().c_str();
				}
				if (it.key() == "onClick")
				{
					item.buttonAttributes.onClick = (char *)it.value().get<string>().c_str();
				}
				if (it.key() == "color")
				{
					item.color = al_map_rgba(it.value()[0].get<int>(), it.value()[1].get<int>(), it.value()[2].get<int>(), it.value()[3].get<int>());
				}
				if (it.key() == "textcolor")
				{
					item.text_color = al_map_rgba(it.value()[0].get<int>(), it.value()[1].get<int>(), it.value()[2].get<int>(), it.value()[3].get<int>());
				}
				result.items.push_back(item);
			}
		}
		size_t lastindex = name.find_last_of(".");
		string rawname = name.substr(0, lastindex);
		string output = "menu/" + rawname + ".rmenu";
		FILE * o;
		fopen_s(&o, output.c_str(), "wb");
		if (o == NULL)
		{
			cout << "File was processed successfully, but the output file couldn't be opened!" << endl;
			return;
		}
		fwrite(rMenuMagic, 1, sizeof(rMenuMagic), o);
		int nSize = sizeof(result.name);
		fwrite(&nSize, sizeof(int), 1, o);
		fwrite(result.name, 1, nSize, o);
		fwrite(&items, sizeof(int), 1, o);
		for (int i = 0; i < items; i++)
		{
			fwrite(&result.items[i], sizeof(result.items[i]), 1, o);
			/*
			cout << result.items[i].rect.X << endl;
			cout << result.items[i].rect.Y << endl;
			cout << result.items[i].rect.W << endl;
			cout << result.items[i].rect.H << endl;
			fwrite(&result.items[i].rect.X, sizeof(int), 1, o);
			fwrite(&result.items[i].rect.Y, sizeof(int), 1, o);
			fwrite(&result.items[i].rect.W, sizeof(int), 1, o);
			fwrite(&result.items[i].rect.H, sizeof(int), 1, o); // rRect_t
			*/
		}
		fclose(o);
	}
	catch (std::domain_error ex)
	{
		cout << ex.what() << endl;
	}
}

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		//PrintUsage();
		//return 0;
	}
	vector<string> input = get_all_files_names_within_folder("menu");
	for each(auto i in input)
	{
		string res;
		res = "menu/" + i;
		//cout << res << endl;
		Process(res, i);
	}
	system("pause");
    return 0;
}

