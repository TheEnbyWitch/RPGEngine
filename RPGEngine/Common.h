#pragma once

#define abort_game(message) { char* s = new char[512]; sprintf(s, "%s:%d -> %s()", __FILENAME__, __LINE__, __func__) ;abort_game_ex(message, s); }

void abort_game_ex(const char* message, const char* header);

void rpge_printf(const char* message, ...);

char * va(const char * t, ...);
