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
	string search_path = folder + "/*.jmenu";
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
		memcpy(result.name, menuName.c_str(), sizeof(result.name));
		for (int i = 0; i < items; i++)
		{
			cout << "Processing item[" << i << "]" << endl;
			json item = inputFile["items"][i];
			rMenuItem_t itemr;
			// text
			memcpy(itemr.text, item["text"].get<string>().c_str(), sizeof(itemr.text));
			cout << "text: " << itemr.text << endl;
			// rect
			itemr.rect.X = item["rect"][0].get<int>();
			itemr.rect.Y = item["rect"][1].get<int>();
			itemr.rect.W = item["rect"][2].get<int>();
			itemr.rect.H = item["rect"][3].get<int>();
			cout << "rect: {" << itemr.rect.X << ", " << itemr.rect.Y << ", " << itemr.rect.W << ", " << itemr.rect.H << "}" << endl;
			// type
			itemr.type = (rMenuItemType)item["type"].get<int>();
			cout << "type: " << itemr.type << endl;
			// color
			itemr.color = al_map_rgba(item["color"][0].get<unsigned char>(), item["color"][1].get<unsigned char>(), item["color"][2].get<unsigned char>(), item["color"][3].get<unsigned char>());
			cout << "color: {" << (int)itemr.color.r << ", " << (int)itemr.color.g << ", " << (int)itemr.color.b << ", " << (int)itemr.color.a << "}" << endl;
			// textcolor
			itemr.text_color = al_map_rgba(item["textcolor"][0].get<unsigned char>(), item["textcolor"][1].get<unsigned char>(), item["textcolor"][2].get<unsigned char>(), item["textcolor"][3].get<unsigned char>());
			cout << "textcolor: {" << (int)itemr.text_color.r << ", " << (int)itemr.text_color.g << ", " << (int)itemr.text_color.b << ", " << (int)itemr.text_color.a << "}" << endl;
			// BUTTON SPECIFIC
			itemr.buttonAttributes.index = item["index"].get<int>();
			memcpy(itemr.buttonAttributes.onClick, item["onClick"].get<string>().c_str(), sizeof(itemr.buttonAttributes.onClick));
			memcpy(itemr.buttonAttributes.onClickArgs, item["onClickArgs"].get<string>().c_str(), sizeof(itemr.buttonAttributes.onClickArgs));
			result.items.push_back(itemr);
		}
		size_t lastindex = name.find_last_of(".");
		string rawname = name.substr(0, lastindex);
		string output = "menu/" + rawname + ".rmenu";
		FILE * o;
		remove(output.c_str());
		fopen_s(&o, output.c_str(), "wb");
		if (o == NULL)
		{
			cout << "File was processed successfully, but the output file couldn't be opened!" << endl;
			return;
		}
		fwrite(rMenuMagic, 1, sizeof(rMenuMagic), o);
		fwrite(result.name, 1, sizeof(result.name), o);
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
	catch (std::exception ex2)
	{
		cout << ex2.what() << endl;
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

