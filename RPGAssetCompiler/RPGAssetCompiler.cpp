// RPGAssetCompiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define ProcessExtension(ext, func) \
if(strrchr(argv[i],'.') != NULL && strcmp(ext, strrchr(argv[i],'.')) == 0) func(argv[i]);

void ProcessDialogue(char *file)
{
	FILE * input;

	FILE * output;
	FILE * string_hash_output;

	fopen_s(&input, file, "rb");
	while (!feof(input))
	{
		char buffer[2048];
		fgets(buffer, 2048, input);
		int begin = 0;
		for (int i = 0; i < 2048; i++)
		{
			if (buffer[i] == ' ' || buffer[i] == '	')
			{
				begin++;
			}
			else {
				break;
			}
		}
		char cmd[2048];
		for (int i = begin; i < 2048; i++)
		{
			if (buffer[i] == ' ' || buffer[i] == '	')
			{
				cmd[i - begin] = '\0';
				begin = i;
				break;
			}
			else {
				cmd[i - begin] = buffer[i - begin];
			}
		}
	}
}

int main(int argc, char * argv[])
{
	for (int i = 1; i < argc; i++)
	{
		ProcessExtension("rdlg", ProcessDialogue);
	}
    return 0;
}

