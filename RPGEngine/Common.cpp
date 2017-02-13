#include "stdafx.h"

std::string consoleLog;
std::string consoleInput;

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
	char *result = new char[1024];
	va_list args;
	va_start(args, t);
	vsprintf(result, t, args);
	va_end(args);
	return result;
}
// MACRO FOR ANY KEY WITHOUT MODIFIERS
#define KC(in, out) if(keycode == ALLEGRO_KEY_##in) return out;

// MACRO FOR CHARACTER KEYS WITH MODIFIERS
#define KCC(in) if(keycode == ALLEGRO_KEY_##in) \
return (#in[0])-(modifiers & ALLEGRO_KEYMOD_SHIFT ? 'A'-'a' : 0 );

char GetCharFromKeycode(int keycode, int modifiers)
{
	if (modifiers != 0) rpge_printf("modifiers: %d\n", modifiers);
	KCC(A);
	KCC(B);
	KCC(C);
	KCC(D);
	KCC(E);
	KCC(F);
	KCC(G);
	KCC(H);
	KCC(I);
	KCC(J);
	KCC(K);
	KCC(L);
	KCC(M);
	KCC(N);
	KCC(O);
	KCC(P);
	KCC(Q);
	KCC(R);
	KCC(S);
	KCC(T);
	KCC(U);
	KCC(V);
	KCC(W);
	KCC(X);
	KCC(Y);
	KCC(Z);
	KC(SPACE, ' ');
	return 0;
}