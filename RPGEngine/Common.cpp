#include "stdafx.h"

std::string consoleLog;

void abort_game_ex(const char* message, const char* header = "The game was forced to abort")
{
	rpge_printf("%s \n", message);
	al_show_native_message_box(al_get_current_display(), "Error", header, message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	exit(1);
}

void rpge_printf(const char * message, ...)
{
	char * result = new char[768];
	va_list args;
	va_start(args, message);
	vsprintf(result, message, args);
	printf(
		"%s", result
	);
	//al_append_native_text_log(txtLog, "%s", result);
	consoleLog += result;
	va_end(args);
}

char * va(const char * t, ...)
{
	char * result = new char[1024];
	va_list args;
	va_start(args, t);
	vsprintf(result, t, args);
	va_end(args);
	return result;
}