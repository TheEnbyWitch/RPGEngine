#include "stdafx.h"

std::string consoleLog;
std::string consoleInput;

size_t find_ext_idx(const char* fileName)
{
	size_t len = strlen(fileName);
	size_t idx = len - 1;
	for (size_t i = 0; *(fileName + i); i++) {
		if (*(fileName + i) == '.') {
			idx = i;
		}
		else if (*(fileName + i) == '/' || *(fileName + i) == '\\') {
			idx = len - 1;
		}
	}
	return idx + 1;
}

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
	static int index = 0;
	static char string[4][1024];	// in case called by nested functions
	char *result;
	result = string[index];
	index = (index + 1) & 3;
	va_list args;
	va_start(args, t);
	vsprintf(result, t, args);
	va_end(args);
	return result;
}

// MACRO FOR ANY KEY WITHOUT MODIFIERS
#define KC(in, out) if(keycode == ALLEGRO_KEY_##in) return out;

#define KC_ST(in, out) if(keycode == ALLEGRO_KEY_##in && (modifiers & ALLEGRO_KEYMOD_SHIFT || modifiers & ALLEGRO_KEYMOD_CAPSLOCK)) return out;
#define KC_SF(in, out) if(keycode == ALLEGRO_KEY_##in && !(modifiers & ALLEGRO_KEYMOD_SHIFT || modifiers & ALLEGRO_KEYMOD_CAPSLOCK)) return out;

// MACRO FOR CHARACTER KEYS WITH MODIFIERS
#define KCC(in) if(keycode == ALLEGRO_KEY_##in) \
return (#in[0])-((modifiers & ALLEGRO_KEYMOD_SHIFT || modifiers & ALLEGRO_KEYMOD_CAPSLOCK) ? 0 : 'A'-'a');

char GetCharFromKeycode(int keycode, int modifiers)
{
	//if (modifiers != 0) rpge_printf("modifiers: %d\n", modifiers);
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

	KC_ST(QUOTE, '"');
	KC_SF(QUOTE, '\'');

	KC_ST(TILDE, '~');
	KC_SF(TILDE, '`');
	KC_ST(MINUS, '_');
	KC_SF(MINUS, '-');
	KC_ST(EQUALS, '+');
	KC_SF(EQUALS, '=');
	KC_ST(OPENBRACE, '{');
	KC_SF(OPENBRACE, '[');
	KC_ST(CLOSEBRACE, '}');
	KC_SF(CLOSEBRACE, ']');

	KC_ST(SEMICOLON, ':');
	KC_SF(SEMICOLON, ';');
	KC_ST(BACKSLASH, '|');
	KC_SF(BACKSLASH, '\\');
	KC_ST(COMMA, '<');
	KC_SF(COMMA, ',');
	KC_ST(FULLSTOP, '>');
	KC_SF(FULLSTOP, '.');
	KC_ST(SLASH, '?');
	KC_SF(SLASH, '/');

	/*
	ALLEGRO_KEY_SEMICOLON	= 68,
ALLEGRO_KEY_QUOTE		= 69,
ALLEGRO_KEY_BACKSLASH	= 70,
ALLEGRO_KEY_BACKSLASH2	= 71,  // DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" 
ALLEGRO_KEY_COMMA = 72,
ALLEGRO_KEY_FULLSTOP = 73,
ALLEGRO_KEY_SLASH = 74,
*/
	
	KC_SF(0, '0');
	KC_SF(1, '1');
	KC_SF(2, '2');
	KC_SF(3, '3');
	KC_SF(4, '4');
	KC_SF(5, '5');
	KC_SF(6, '6');
	KC_SF(7, '7');
	KC_SF(8, '8');
	KC_SF(9, '9');

	KC_ST(0, ')');
	KC_ST(1, '!');
	KC_ST(2, '@');
	KC_ST(3, '#');
	KC_ST(4, '$');
	KC_ST(5, '%');
	KC_ST(6, '^');
	KC_ST(7, '&');
	KC_ST(8, '*');
	KC_ST(9, '(');
	
	return 0;
}

/*
enum
{
ALLEGRO_KEY_A		= 1,
ALLEGRO_KEY_B		= 2,
ALLEGRO_KEY_C		= 3,
ALLEGRO_KEY_D		= 4,
ALLEGRO_KEY_E		= 5,
ALLEGRO_KEY_F		= 6,
ALLEGRO_KEY_G		= 7,
ALLEGRO_KEY_H		= 8,
ALLEGRO_KEY_I		= 9,
ALLEGRO_KEY_J		= 10,
ALLEGRO_KEY_K		= 11,
ALLEGRO_KEY_L		= 12,
ALLEGRO_KEY_M		= 13,
ALLEGRO_KEY_N		= 14,
ALLEGRO_KEY_O		= 15,
ALLEGRO_KEY_P		= 16,
ALLEGRO_KEY_Q		= 17,
ALLEGRO_KEY_R		= 18,
ALLEGRO_KEY_S		= 19,
ALLEGRO_KEY_T		= 20,
ALLEGRO_KEY_U		= 21,
ALLEGRO_KEY_V		= 22,
ALLEGRO_KEY_W		= 23,
ALLEGRO_KEY_X		= 24,
ALLEGRO_KEY_Y		= 25,
ALLEGRO_KEY_Z		= 26,

ALLEGRO_KEY_0		= 27,
ALLEGRO_KEY_1		= 28,
ALLEGRO_KEY_2		= 29,
ALLEGRO_KEY_3		= 30,
ALLEGRO_KEY_4		= 31,
ALLEGRO_KEY_5		= 32,
ALLEGRO_KEY_6		= 33,
ALLEGRO_KEY_7		= 34,
ALLEGRO_KEY_8		= 35,
ALLEGRO_KEY_9		= 36,

ALLEGRO_KEY_PAD_0		= 37,
ALLEGRO_KEY_PAD_1		= 38,
ALLEGRO_KEY_PAD_2		= 39,
ALLEGRO_KEY_PAD_3		= 40,
ALLEGRO_KEY_PAD_4		= 41,
ALLEGRO_KEY_PAD_5		= 42,
ALLEGRO_KEY_PAD_6		= 43,
ALLEGRO_KEY_PAD_7		= 44,
ALLEGRO_KEY_PAD_8		= 45,
ALLEGRO_KEY_PAD_9		= 46,

ALLEGRO_KEY_F1		= 47,
ALLEGRO_KEY_F2		= 48,
ALLEGRO_KEY_F3		= 49,
ALLEGRO_KEY_F4		= 50,
ALLEGRO_KEY_F5		= 51,
ALLEGRO_KEY_F6		= 52,
ALLEGRO_KEY_F7		= 53,
ALLEGRO_KEY_F8		= 54,
ALLEGRO_KEY_F9		= 55,
ALLEGRO_KEY_F10		= 56,
ALLEGRO_KEY_F11		= 57,
ALLEGRO_KEY_F12		= 58,

ALLEGRO_KEY_ESCAPE	= 59,
ALLEGRO_KEY_TILDE		= 60,
ALLEGRO_KEY_MINUS		= 61,
ALLEGRO_KEY_EQUALS	= 62,
ALLEGRO_KEY_BACKSPACE	= 63,
ALLEGRO_KEY_TAB		= 64,
ALLEGRO_KEY_OPENBRACE	= 65,
ALLEGRO_KEY_CLOSEBRACE	= 66,
ALLEGRO_KEY_ENTER		= 67,
ALLEGRO_KEY_SEMICOLON	= 68,
ALLEGRO_KEY_QUOTE		= 69,
ALLEGRO_KEY_BACKSLASH	= 70,
ALLEGRO_KEY_BACKSLASH2	= 71,  // DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" 
ALLEGRO_KEY_COMMA = 72,
ALLEGRO_KEY_FULLSTOP = 73,
ALLEGRO_KEY_SLASH = 74,
ALLEGRO_KEY_SPACE = 75,

ALLEGRO_KEY_INSERT = 76,
ALLEGRO_KEY_DELETE = 77,
ALLEGRO_KEY_HOME = 78,
ALLEGRO_KEY_END = 79,
ALLEGRO_KEY_PGUP = 80,
ALLEGRO_KEY_PGDN = 81,
ALLEGRO_KEY_LEFT = 82,
ALLEGRO_KEY_RIGHT = 83,
ALLEGRO_KEY_UP = 84,
ALLEGRO_KEY_DOWN = 85,

ALLEGRO_KEY_PAD_SLASH = 86,
ALLEGRO_KEY_PAD_ASTERISK = 87,
ALLEGRO_KEY_PAD_MINUS = 88,
ALLEGRO_KEY_PAD_PLUS = 89,
ALLEGRO_KEY_PAD_DELETE = 90,
ALLEGRO_KEY_PAD_ENTER = 91,

ALLEGRO_KEY_PRINTSCREEN = 92,
ALLEGRO_KEY_PAUSE = 93,

ALLEGRO_KEY_ABNT_C1 = 94,
ALLEGRO_KEY_YEN = 95,
ALLEGRO_KEY_KANA = 96,
ALLEGRO_KEY_CONVERT = 97,
ALLEGRO_KEY_NOCONVERT = 98,
ALLEGRO_KEY_AT = 99,
ALLEGRO_KEY_CIRCUMFLEX = 100,
ALLEGRO_KEY_COLON2 = 101,
ALLEGRO_KEY_KANJI = 102,

ALLEGRO_KEY_PAD_EQUALS = 103,	
ALLEGRO_KEY_BACKQUOTE = 104,	
ALLEGRO_KEY_SEMICOLON2 = 105,	
ALLEGRO_KEY_COMMAND = 106,	

ALLEGRO_KEY_BACK = 107,        
ALLEGRO_KEY_VOLUME_UP = 108,
ALLEGRO_KEY_VOLUME_DOWN = 109,

ALLEGRO_KEY_SEARCH = 110,
ALLEGRO_KEY_DPAD_CENTER = 111,
ALLEGRO_KEY_BUTTON_X = 112,
ALLEGRO_KEY_BUTTON_Y = 113,
ALLEGRO_KEY_DPAD_UP = 114,
ALLEGRO_KEY_DPAD_DOWN = 115,
ALLEGRO_KEY_DPAD_LEFT = 116,
ALLEGRO_KEY_DPAD_RIGHT = 117,
ALLEGRO_KEY_SELECT = 118,
ALLEGRO_KEY_START = 119,
ALLEGRO_KEY_BUTTON_L1 = 120,
ALLEGRO_KEY_BUTTON_R1 = 121,
ALLEGRO_KEY_BUTTON_L2 = 122,
ALLEGRO_KEY_BUTTON_R2 = 123,
ALLEGRO_KEY_BUTTON_A = 124,
ALLEGRO_KEY_BUTTON_B = 125,
ALLEGRO_KEY_THUMBL = 126,
ALLEGRO_KEY_THUMBR = 127,

ALLEGRO_KEY_UNKNOWN = 128,

ALLEGRO_KEY_MODIFIERS = 215,

ALLEGRO_KEY_LSHIFT = 215,
ALLEGRO_KEY_RSHIFT = 216,
ALLEGRO_KEY_LCTRL = 217,
ALLEGRO_KEY_RCTRL = 218,
ALLEGRO_KEY_ALT = 219,
ALLEGRO_KEY_ALTGR = 220,
ALLEGRO_KEY_LWIN = 221,
ALLEGRO_KEY_RWIN = 222,
ALLEGRO_KEY_MENU = 223,
ALLEGRO_KEY_SCROLLLOCK = 224,
ALLEGRO_KEY_NUMLOCK = 225,
ALLEGRO_KEY_CAPSLOCK = 226,

ALLEGRO_KEY_MAX
}; 
*/