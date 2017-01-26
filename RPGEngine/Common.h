#pragma once

typedef struct rRect_s {
	int X;
	int Y;
	int W;
	int H;
} rRect_t;

typedef struct rScaledRegion_s {
	int sourceX;
	int sourceY;
	int sourceW;
	int sourceH;
	int destinationX;
	int destinationY;
	int destinationW;
	int destinationH;
} rScaledRegion_t;
//float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh


#define abort_game(message) { char* s = new char[512]; sprintf(s, "%s:%d -> %s()", __FILENAME__, __LINE__, __func__) ;abort_game_ex(message, s); }

void abort_game_ex(const char* message, const char* header);

void rpge_printf(const char* message, ...);

char * va(const char * t, ...);
