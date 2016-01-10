#include "key.h"
#include "window.h"
#include "struct_wrappers.h"
#include "container.h"

using namespace v8;
using namespace node;


void sdl::key::Init(Handle<Object> exports) {
	HandleScope scope;

	Handle<Object> KEY = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("KEY"), KEY);
	// Keys are set in the order that they are listed on this page:
	//  http://wiki.libsdl.org/SDLKeycodeLookup
	Nan::Set(KEY, Nan::New<String>("BACKSPACE"), Nan::New<Number>(SDLK_BACKSPACE));
	Nan::Set(KEY, Nan::New<String>("TAB"), Nan::New<Number>(SDLK_BACKSPACE));
	Nan::Set(KEY, Nan::New<String>("RETURN"), Nan::New<Number>(SDLK_RETURN));
	Nan::Set(KEY, Nan::New<String>("ESCAPE"), Nan::New<Number>(SDLK_ESCAPE));
	Nan::Set(KEY, Nan::New<String>("SPACE"), Nan::New<Number>(SDLK_SPACE));
	Nan::Set(KEY, Nan::New<String>("EXCLAIM"), Nan::New<Number>(SDLK_EXCLAIM));
	Nan::Set(KEY, Nan::New<String>("QUOTEDBL"), Nan::New<Number>(SDLK_QUOTEDBL));
	Nan::Set(KEY, Nan::New<String>("HASH"), Nan::New<Number>(SDLK_HASH));
	Nan::Set(KEY, Nan::New<String>("DOLLAR"), Nan::New<Number>(SDLK_DOLLAR));
	Nan::Set(KEY, Nan::New<String>("PERCENT"), Nan::New<Number>(SDLK_PERCENT));
	Nan::Set(KEY, Nan::New<String>("APERSAND"), Nan::New<Number>(SDLK_AMPERSAND));
	Nan::Set(KEY, Nan::New<String>("QUOTE"), Nan::New<Number>(SDLK_QUOTE));
	Nan::Set(KEY, Nan::New<String>("LEFTPAREN"), Nan::New<Number>(SDLK_LEFTPAREN));
	Nan::Set(KEY, Nan::New<String>("RIGHTPAREN"), Nan::New<Number>(SDLK_RIGHTPAREN));
	Nan::Set(KEY, Nan::New<String>("ASTERISK"), Nan::New<Number>(SDLK_ASTERISK));
	Nan::Set(KEY, Nan::New<String>("PLUS"), Nan::New<Number>(SDLK_PLUS));
	Nan::Set(KEY, Nan::New<String>("COMMA"), Nan::New<Number>(SDLK_COMMA));
	Nan::Set(KEY, Nan::New<String>("MINUS"), Nan::New<Number>(SDLK_MINUS));
	Nan::Set(KEY, Nan::New<String>("PERIOD"), Nan::New<Number>(SDLK_PERIOD));
	Nan::Set(KEY, Nan::New<String>("SLASH"), Nan::New<Number>(SDLK_SLASH));
	// Number keys. Numbers are written out due to being unable to write:
	//   foo.0 = "bar";
	// (even if you can do: foo[0] = "bar", it just isn't as friendly)
	Nan::Set(KEY, Nan::New<String>("ZERO"), Nan::New<Number>(SDLK_0));
	Nan::Set(KEY, Nan::New<String>("ONE"), Nan::New<Number>(SDLK_1));
	Nan::Set(KEY, Nan::New<String>("TWO"), Nan::New<Number>(SDLK_2));
	Nan::Set(KEY, Nan::New<String>("THREE"), Nan::New<Number>(SDLK_3));
	Nan::Set(KEY, Nan::New<String>("FOUR"), Nan::New<Number>(SDLK_4));
	Nan::Set(KEY, Nan::New<String>("FIVE"), Nan::New<Number>(SDLK_5));
	Nan::Set(KEY, Nan::New<String>("SIX"), Nan::New<Number>(SDLK_6));
	Nan::Set(KEY, Nan::New<String>("SEVEN"), Nan::New<Number>(SDLK_7));
	Nan::Set(KEY, Nan::New<String>("EIGHT"), Nan::New<Number>(SDLK_8));
	Nan::Set(KEY, Nan::New<String>("NINE"), Nan::New<Number>(SDLK_9));
	Nan::Set(KEY, Nan::New<String>("COLON"), Nan::New<Number>(SDLK_COLON));
	Nan::Set(KEY, Nan::New<String>("SEMICOLON"), Nan::New<Number>(SDLK_SEMICOLON));
	Nan::Set(KEY, Nan::New<String>("LESS"), Nan::New<Number>(SDLK_LESS));
	Nan::Set(KEY, Nan::New<String>("EQUALS"), Nan::New<Number>(SDLK_EQUALS));
	Nan::Set(KEY, Nan::New<String>("GREATER"), Nan::New<Number>(SDLK_GREATER));
	Nan::Set(KEY, Nan::New<String>("QUESTION"), Nan::New<Number>(SDLK_QUESTION));
	Nan::Set(KEY, Nan::New<String>("AT"), Nan::New<Number>(SDLK_AT));
	Nan::Set(KEY, Nan::New<String>("LEFTBRACKET"), Nan::New<Number>(SDLK_LEFTBRACKET));
	Nan::Set(KEY, Nan::New<String>("BACKSLASH"), Nan::New<Number>(SDLK_BACKSLASH));
	Nan::Set(KEY, Nan::New<String>("RIGHTBRACKET"), Nan::New<Number>(SDLK_RIGHTBRACKET));
	Nan::Set(KEY, Nan::New<String>("CARET"), Nan::New<Number>(SDLK_CARET));
	Nan::Set(KEY, Nan::New<String>("UNDERSCORE"), Nan::New<Number>(SDLK_UNDERSCORE));
	Nan::Set(KEY, Nan::New<String>("BACKQUOTE"), Nan::New<Number>(SDLK_BACKQUOTE));
	Nan::Set(KEY, Nan::New<String>("A"), Nan::New<Number>(SDLK_a));
	Nan::Set(KEY, Nan::New<String>("B"), Nan::New<Number>(SDLK_b));
	Nan::Set(KEY, Nan::New<String>("C"), Nan::New<Number>(SDLK_c));
	Nan::Set(KEY, Nan::New<String>("D"), Nan::New<Number>(SDLK_d));
	Nan::Set(KEY, Nan::New<String>("E"), Nan::New<Number>(SDLK_e));
	Nan::Set(KEY, Nan::New<String>("F"), Nan::New<Number>(SDLK_f));
	Nan::Set(KEY, Nan::New<String>("G"), Nan::New<Number>(SDLK_g));
	Nan::Set(KEY, Nan::New<String>("H"), Nan::New<Number>(SDLK_h));
	Nan::Set(KEY, Nan::New<String>("I"), Nan::New<Number>(SDLK_i));
	Nan::Set(KEY, Nan::New<String>("J"), Nan::New<Number>(SDLK_j));
	Nan::Set(KEY, Nan::New<String>("K"), Nan::New<Number>(SDLK_k));
	Nan::Set(KEY, Nan::New<String>("L"), Nan::New<Number>(SDLK_l));
	Nan::Set(KEY, Nan::New<String>("M"), Nan::New<Number>(SDLK_m));
	Nan::Set(KEY, Nan::New<String>("N"), Nan::New<Number>(SDLK_n));
	Nan::Set(KEY, Nan::New<String>("O"), Nan::New<Number>(SDLK_o));
	Nan::Set(KEY, Nan::New<String>("P"), Nan::New<Number>(SDLK_p));
	Nan::Set(KEY, Nan::New<String>("Q"), Nan::New<Number>(SDLK_q));
	Nan::Set(KEY, Nan::New<String>("R"), Nan::New<Number>(SDLK_r));
	Nan::Set(KEY, Nan::New<String>("S"), Nan::New<Number>(SDLK_s));
	Nan::Set(KEY, Nan::New<String>("T"), Nan::New<Number>(SDLK_t));
	Nan::Set(KEY, Nan::New<String>("U"), Nan::New<Number>(SDLK_u));
	Nan::Set(KEY, Nan::New<String>("V"), Nan::New<Number>(SDLK_v));
	Nan::Set(KEY, Nan::New<String>("W"), Nan::New<Number>(SDLK_w));
	Nan::Set(KEY, Nan::New<String>("X"), Nan::New<Number>(SDLK_x));
	Nan::Set(KEY, Nan::New<String>("Y"), Nan::New<Number>(SDLK_y));
	Nan::Set(KEY, Nan::New<String>("Z"), Nan::New<Number>(SDLK_z));
	Nan::Set(KEY, Nan::New<String>("DELETE"), Nan::New<Number>(SDLK_DELETE));
	Nan::Set(KEY, Nan::New<String>("CAPSLOCK"), Nan::New<Number>(SDLK_CAPSLOCK));
	Nan::Set(KEY, Nan::New<String>("F1"), Nan::New<Number>(SDLK_F1));
	Nan::Set(KEY, Nan::New<String>("F2"), Nan::New<Number>(SDLK_F2));
	Nan::Set(KEY, Nan::New<String>("F3"), Nan::New<Number>(SDLK_F3));
	Nan::Set(KEY, Nan::New<String>("F4"), Nan::New<Number>(SDLK_F4));
	Nan::Set(KEY, Nan::New<String>("F5"), Nan::New<Number>(SDLK_F5));
	Nan::Set(KEY, Nan::New<String>("F6"), Nan::New<Number>(SDLK_F6));
	Nan::Set(KEY, Nan::New<String>("F7"), Nan::New<Number>(SDLK_F7));
	Nan::Set(KEY, Nan::New<String>("F8"), Nan::New<Number>(SDLK_F8));
	Nan::Set(KEY, Nan::New<String>("F9"), Nan::New<Number>(SDLK_F9));
	Nan::Set(KEY, Nan::New<String>("F10"), Nan::New<Number>(SDLK_F10));
	Nan::Set(KEY, Nan::New<String>("F11"), Nan::New<Number>(SDLK_F11));
	Nan::Set(KEY, Nan::New<String>("F12"), Nan::New<Number>(SDLK_F12));
	Nan::Set(KEY, Nan::New<String>("PRINTSCREEN"), Nan::New<Number>(SDLK_PRINTSCREEN));
	Nan::Set(KEY, Nan::New<String>("SCROLLLOCK"), Nan::New<Number>(SDLK_SCROLLLOCK));
	Nan::Set(KEY, Nan::New<String>("PAUSE"), Nan::New<Number>(SDLK_PAUSE));
	Nan::Set(KEY, Nan::New<String>("INSERT"), Nan::New<Number>(SDLK_INSERT));
	Nan::Set(KEY, Nan::New<String>("HOME"), Nan::New<Number>(SDLK_HOME));
	Nan::Set(KEY, Nan::New<String>("PAGEUP"), Nan::New<Number>(SDLK_PAGEUP));
	Nan::Set(KEY, Nan::New<String>("END"), Nan::New<Number>(SDLK_END));
	Nan::Set(KEY, Nan::New<String>("PAGEDOWN"), Nan::New<Number>(SDLK_PAGEDOWN));
	Nan::Set(KEY, Nan::New<String>("RIGHT"), Nan::New<Number>(SDLK_RIGHT));
	Nan::Set(KEY, Nan::New<String>("LEFT"), Nan::New<Number>(SDLK_LEFT));
	Nan::Set(KEY, Nan::New<String>("DOWN"), Nan::New<Number>(SDLK_DOWN));
	Nan::Set(KEY, Nan::New<String>("UP"), Nan::New<Number>(SDLK_UP));
	Nan::Set(KEY, Nan::New<String>("HOME"), Nan::New<Number>(SDLK_HOME));
	Nan::Set(KEY, Nan::New<String>("NUMLOCKCLEAR"), Nan::New<Number>(SDLK_NUMLOCKCLEAR));
	Nan::Set(KEY, Nan::New<String>("KP_DIVIDE"), Nan::New<Number>(SDLK_KP_DIVIDE));
	Nan::Set(KEY, Nan::New<String>("KP_MULTIPLY"), Nan::New<Number>(SDLK_KP_MULTIPLY));
	Nan::Set(KEY, Nan::New<String>("KP_MINUS"), Nan::New<Number>(SDLK_KP_MINUS));
	Nan::Set(KEY, Nan::New<String>("KP_PLUS"), Nan::New<Number>(SDLK_KP_PLUS));
	Nan::Set(KEY, Nan::New<String>("KP_ENTER"), Nan::New<Number>(SDLK_KP_ENTER));
	Nan::Set(KEY, Nan::New<String>("KP_ONE"), Nan::New<Number>(SDLK_KP_1));
	Nan::Set(KEY, Nan::New<String>("KP_TWO"), Nan::New<Number>(SDLK_KP_2));
	Nan::Set(KEY, Nan::New<String>("KP_THREE"), Nan::New<Number>(SDLK_KP_3));
	Nan::Set(KEY, Nan::New<String>("KP_FOUR"), Nan::New<Number>(SDLK_KP_4));
	Nan::Set(KEY, Nan::New<String>("KP_FIVE"), Nan::New<Number>(SDLK_KP_5));
	Nan::Set(KEY, Nan::New<String>("KP_SIX"), Nan::New<Number>(SDLK_KP_6));
	Nan::Set(KEY, Nan::New<String>("KP_SEVEN"), Nan::New<Number>(SDLK_KP_7));
	Nan::Set(KEY, Nan::New<String>("KP_EIGHT"), Nan::New<Number>(SDLK_KP_8));
	Nan::Set(KEY, Nan::New<String>("KP_NINE"), Nan::New<Number>(SDLK_KP_9));
	Nan::Set(KEY, Nan::New<String>("KP_ZERO"), Nan::New<Number>(SDLK_KP_0));
	Nan::Set(KEY, Nan::New<String>("KP_PERIOD"), Nan::New<Number>(SDLK_KP_PERIOD));
	Nan::Set(KEY, Nan::New<String>("APPLICATION"), Nan::New<Number>(SDLK_APPLICATION));
	Nan::Set(KEY, Nan::New<String>("POWER"), Nan::New<Number>(SDLK_POWER));
	Nan::Set(KEY, Nan::New<String>("KP_EQUALS"), Nan::New<Number>(SDLK_KP_EQUALS));
	Nan::Set(KEY, Nan::New<String>("F13"), Nan::New<Number>(SDLK_F13));
	Nan::Set(KEY, Nan::New<String>("F14"), Nan::New<Number>(SDLK_F14));
	Nan::Set(KEY, Nan::New<String>("F15"), Nan::New<Number>(SDLK_F15));
	Nan::Set(KEY, Nan::New<String>("F16"), Nan::New<Number>(SDLK_F16));
	Nan::Set(KEY, Nan::New<String>("F17"), Nan::New<Number>(SDLK_F17));
	Nan::Set(KEY, Nan::New<String>("F18"), Nan::New<Number>(SDLK_F18));
	Nan::Set(KEY, Nan::New<String>("F19"), Nan::New<Number>(SDLK_F19));
	Nan::Set(KEY, Nan::New<String>("F20"), Nan::New<Number>(SDLK_F20));
	Nan::Set(KEY, Nan::New<String>("F21"), Nan::New<Number>(SDLK_F21));
	Nan::Set(KEY, Nan::New<String>("F22"), Nan::New<Number>(SDLK_F22));
	Nan::Set(KEY, Nan::New<String>("F23"), Nan::New<Number>(SDLK_F23));
	Nan::Set(KEY, Nan::New<String>("F24"), Nan::New<Number>(SDLK_F24));
	Nan::Set(KEY, Nan::New<String>("EXECUTE"), Nan::New<Number>(SDLK_EXECUTE));
	Nan::Set(KEY, Nan::New<String>("HELP"), Nan::New<Number>(SDLK_HELP));
	Nan::Set(KEY, Nan::New<String>("MENU"), Nan::New<Number>(SDLK_MENU));
	Nan::Set(KEY, Nan::New<String>("SELECT"), Nan::New<Number>(SDLK_SELECT));
	Nan::Set(KEY, Nan::New<String>("STOP"), Nan::New<Number>(SDLK_STOP));
	Nan::Set(KEY, Nan::New<String>("AGAIN"), Nan::New<Number>(SDLK_AGAIN));
	Nan::Set(KEY, Nan::New<String>("UNDO"), Nan::New<Number>(SDLK_UNDO));
	Nan::Set(KEY, Nan::New<String>("CUT"), Nan::New<Number>(SDLK_CUT));
	Nan::Set(KEY, Nan::New<String>("COPY"), Nan::New<Number>(SDLK_COPY));
	Nan::Set(KEY, Nan::New<String>("PASTE"), Nan::New<Number>(SDLK_PASTE));
	Nan::Set(KEY, Nan::New<String>("FIND"), Nan::New<Number>(SDLK_FIND));
	Nan::Set(KEY, Nan::New<String>("MUTE"), Nan::New<Number>(SDLK_MUTE));
	Nan::Set(KEY, Nan::New<String>("VOLUMEUP"), Nan::New<Number>(SDLK_VOLUMEUP));
	Nan::Set(KEY, Nan::New<String>("VOLUMEDOWN"), Nan::New<Number>(SDLK_VOLUMEDOWN));
	Nan::Set(KEY, Nan::New<String>("KP_COMMA"), Nan::New<Number>(SDLK_KP_COMMA));
	Nan::Set(KEY, Nan::New<String>("KP_EQUALSAS400"), Nan::New<Number>(SDLK_KP_EQUALSAS400));
	Nan::Set(KEY, Nan::New<String>("ALTERASE"), Nan::New<Number>(SDLK_ALTERASE));
	Nan::Set(KEY, Nan::New<String>("SYSREQ"), Nan::New<Number>(SDLK_SYSREQ));
	Nan::Set(KEY, Nan::New<String>("CANCEL"), Nan::New<Number>(SDLK_CANCEL));
	Nan::Set(KEY, Nan::New<String>("CLEAR"), Nan::New<Number>(SDLK_CLEAR));
	Nan::Set(KEY, Nan::New<String>("PRIOR"), Nan::New<Number>(SDLK_PRIOR));
	Nan::Set(KEY, Nan::New<String>("RETURN2"), Nan::New<Number>(SDLK_RETURN2));
	Nan::Set(KEY, Nan::New<String>("SEPARATOR"), Nan::New<Number>(SDLK_SEPARATOR));
	Nan::Set(KEY, Nan::New<String>("OUT"), Nan::New<Number>(SDLK_OUT));
	Nan::Set(KEY, Nan::New<String>("OPER"), Nan::New<Number>(SDLK_OPER));
	Nan::Set(KEY, Nan::New<String>("CLEARAGAIN"), Nan::New<Number>(SDLK_CLEARAGAIN));
	Nan::Set(KEY, Nan::New<String>("CRSEL"), Nan::New<Number>(SDLK_CRSEL));
	Nan::Set(KEY, Nan::New<String>("EXSEL"), Nan::New<Number>(SDLK_EXSEL));
	Nan::Set(KEY, Nan::New<String>("KP_ZEROZERO"), Nan::New<Number>(SDLK_KP_00));
	Nan::Set(KEY, Nan::New<String>("KP_ZEROZEROZERO"), Nan::New<Number>(SDLK_KP_000));
	Nan::Set(KEY, Nan::New<String>("THOUSANDSSEPARATOR"), Nan::New<Number>(SDLK_THOUSANDSSEPARATOR));
	Nan::Set(KEY, Nan::New<String>("DECIMALSEPARATOR"), Nan::New<Number>(SDLK_DECIMALSEPARATOR));
	Nan::Set(KEY, Nan::New<String>("CURRENCYUNIT"), Nan::New<Number>(SDLK_CURRENCYUNIT));
	Nan::Set(KEY, Nan::New<String>("CURRENCYSUBUNIT"), Nan::New<Number>(SDLK_CURRENCYSUBUNIT));
	Nan::Set(KEY, Nan::New<String>("KP_LEFTPAREN"), Nan::New<Number>(SDLK_KP_LEFTPAREN));
	Nan::Set(KEY, Nan::New<String>("KP_RIGHTPAREN"), Nan::New<Number>(SDLK_KP_RIGHTPAREN));
	Nan::Set(KEY, Nan::New<String>("KP_LEFTBRACE"), Nan::New<Number>(SDLK_KP_LEFTBRACE));
	Nan::Set(KEY, Nan::New<String>("KP_RIGHTBRACE"), Nan::New<Number>(SDLK_KP_RIGHTBRACE));
	Nan::Set(KEY, Nan::New<String>("KP_TAB"), Nan::New<Number>(SDLK_KP_TAB));
	Nan::Set(KEY, Nan::New<String>("KP_BACKSPACE"), Nan::New<Number>(SDLK_KP_BACKSPACE));
	Nan::Set(KEY, Nan::New<String>("KP_A"), Nan::New<Number>(SDLK_KP_A));
	Nan::Set(KEY, Nan::New<String>("KP_B"), Nan::New<Number>(SDLK_KP_B));
	Nan::Set(KEY, Nan::New<String>("KP_C"), Nan::New<Number>(SDLK_KP_C));
	Nan::Set(KEY, Nan::New<String>("KP_D"), Nan::New<Number>(SDLK_KP_D));
	Nan::Set(KEY, Nan::New<String>("KP_E"), Nan::New<Number>(SDLK_KP_E));
	Nan::Set(KEY, Nan::New<String>("KP_F"), Nan::New<Number>(SDLK_KP_F));
	Nan::Set(KEY, Nan::New<String>("KP_XOR"), Nan::New<Number>(SDLK_KP_XOR));
	Nan::Set(KEY, Nan::New<String>("KP_POWER"), Nan::New<Number>(SDLK_KP_POWER));
	Nan::Set(KEY, Nan::New<String>("KP_PERCENT"), Nan::New<Number>(SDLK_KP_PERCENT));
	Nan::Set(KEY, Nan::New<String>("KP_LESS"), Nan::New<Number>(SDLK_KP_LESS));
	Nan::Set(KEY, Nan::New<String>("KP_GREATER"), Nan::New<Number>(SDLK_KP_GREATER));
	Nan::Set(KEY, Nan::New<String>("KP_AMPERSAND"), Nan::New<Number>(SDLK_KP_AMPERSAND));
	Nan::Set(KEY, Nan::New<String>("KP_DBLAMPERSAND"), Nan::New<Number>(SDLK_KP_DBLAMPERSAND));
	Nan::Set(KEY, Nan::New<String>("KP_VERTICALBAR"), Nan::New<Number>(SDLK_KP_VERTICALBAR));
	Nan::Set(KEY, Nan::New<String>("KP_DBLVERTICALBAR"), Nan::New<Number>(SDLK_KP_DBLVERTICALBAR));
	Nan::Set(KEY, Nan::New<String>("KP_COLON"), Nan::New<Number>(SDLK_KP_COLON));
	Nan::Set(KEY, Nan::New<String>("KP_HASH"), Nan::New<Number>(SDLK_KP_HASH));
	Nan::Set(KEY, Nan::New<String>("KP_SPACE"), Nan::New<Number>(SDLK_KP_SPACE));
	Nan::Set(KEY, Nan::New<String>("KP_AT"), Nan::New<Number>(SDLK_KP_AT));
	Nan::Set(KEY, Nan::New<String>("KP_EXCLAM"), Nan::New<Number>(SDLK_KP_EXCLAM));
	Nan::Set(KEY, Nan::New<String>("KP_MEMSTORE"), Nan::New<Number>(SDLK_KP_MEMSTORE));
	Nan::Set(KEY, Nan::New<String>("KP_MEMRECALL"), Nan::New<Number>(SDLK_KP_MEMRECALL));
	Nan::Set(KEY, Nan::New<String>("KP_MEMCLEAR"), Nan::New<Number>(SDLK_KP_MEMCLEAR));
	Nan::Set(KEY, Nan::New<String>("KP_MEMADD"), Nan::New<Number>(SDLK_KP_MEMADD));
	Nan::Set(KEY, Nan::New<String>("KP_MEMSUBTRACT"), Nan::New<Number>(SDLK_KP_MEMSUBTRACT));
	Nan::Set(KEY, Nan::New<String>("KP_MEMMULTIPLY"), Nan::New<Number>(SDLK_KP_MEMMULTIPLY));
	Nan::Set(KEY, Nan::New<String>("KP_MEMDIVIDE"), Nan::New<Number>(SDLK_KP_MEMDIVIDE));
	Nan::Set(KEY, Nan::New<String>("KP_PLUSMINUS"), Nan::New<Number>(SDLK_KP_PLUSMINUS));
	Nan::Set(KEY, Nan::New<String>("KP_CLEAR"), Nan::New<Number>(SDLK_KP_CLEAR));
	Nan::Set(KEY, Nan::New<String>("KP_CLEARENTRY"), Nan::New<Number>(SDLK_KP_CLEARENTRY));
	Nan::Set(KEY, Nan::New<String>("KP_BINARY"), Nan::New<Number>(SDLK_KP_BINARY));
	Nan::Set(KEY, Nan::New<String>("KP_OCTAL"), Nan::New<Number>(SDLK_KP_OCTAL));
	Nan::Set(KEY, Nan::New<String>("KP_DECIMAL"), Nan::New<Number>(SDLK_KP_DECIMAL));
	Nan::Set(KEY, Nan::New<String>("KP_HEXADECIMAL"), Nan::New<Number>(SDLK_KP_HEXADECIMAL));
	Nan::Set(KEY, Nan::New<String>("LCTRL"), Nan::New<Number>(SDLK_LCTRL));
	Nan::Set(KEY, Nan::New<String>("LSHIFT"), Nan::New<Number>(SDLK_LSHIFT));
	Nan::Set(KEY, Nan::New<String>("LALT"), Nan::New<Number>(SDLK_LALT));
	Nan::Set(KEY, Nan::New<String>("LGUI"), Nan::New<Number>(SDLK_LGUI));
	Nan::Set(KEY, Nan::New<String>("RCTRL"), Nan::New<Number>(SDLK_RCTRL));
	Nan::Set(KEY, Nan::New<String>("RSHIFT"), Nan::New<Number>(SDLK_RSHIFT));
	Nan::Set(KEY, Nan::New<String>("RALT"), Nan::New<Number>(SDLK_RALT));
	Nan::Set(KEY, Nan::New<String>("RGUI"), Nan::New<Number>(SDLK_RGUI));
	Nan::Set(KEY, Nan::New<String>("MODE"), Nan::New<Number>(SDLK_MODE));
	Nan::Set(KEY, Nan::New<String>("AUDIONEXT"), Nan::New<Number>(SDLK_AUDIONEXT));
	Nan::Set(KEY, Nan::New<String>("AUDIOPREV"), Nan::New<Number>(SDLK_AUDIOPREV));
	Nan::Set(KEY, Nan::New<String>("AUDIOSTOP"), Nan::New<Number>(SDLK_AUDIOSTOP));
	Nan::Set(KEY, Nan::New<String>("AUDIOPLAY"), Nan::New<Number>(SDLK_AUDIOPLAY));
	Nan::Set(KEY, Nan::New<String>("AUDIOMUTE"), Nan::New<Number>(SDLK_AUDIOMUTE));
	Nan::Set(KEY, Nan::New<String>("MEDIASELECT"), Nan::New<Number>(SDLK_MEDIASELECT));
	Nan::Set(KEY, Nan::New<String>("WWW"), Nan::New<Number>(SDLK_WWW));
	Nan::Set(KEY, Nan::New<String>("MAIL"), Nan::New<Number>(SDLK_MAIL));
	Nan::Set(KEY, Nan::New<String>("CALCULATOR"), Nan::New<Number>(SDLK_CALCULATOR));
	Nan::Set(KEY, Nan::New<String>("COMPUTER"), Nan::New<Number>(SDLK_COMPUTER));
	Nan::Set(KEY, Nan::New<String>("AC_SEARCH"), Nan::New<Number>(SDLK_AC_SEARCH));
	Nan::Set(KEY, Nan::New<String>("AC_HOME"), Nan::New<Number>(SDLK_AC_HOME));
	Nan::Set(KEY, Nan::New<String>("AC_BACK"), Nan::New<Number>(SDLK_AC_BACK));
	Nan::Set(KEY, Nan::New<String>("AC_FORWARD"), Nan::New<Number>(SDLK_AC_FORWARD));
	Nan::Set(KEY, Nan::New<String>("AC_STOP"), Nan::New<Number>(SDLK_AC_STOP));
	Nan::Set(KEY, Nan::New<String>("AC_REFRESH"), Nan::New<Number>(SDLK_AC_REFRESH));
	Nan::Set(KEY, Nan::New<String>("AC_BOOKMARKS"), Nan::New<Number>(SDLK_AC_BOOKMARKS));
	Nan::Set(KEY, Nan::New<String>("BRIGHTNESSDOWN"), Nan::New<Number>(SDLK_BRIGHTNESSDOWN));
	Nan::Set(KEY, Nan::New<String>("BRIGHTNESSUP"), Nan::New<Number>(SDLK_BRIGHTNESSUP));
	Nan::Set(KEY, Nan::New<String>("DISPLAYSWITCH"), Nan::New<Number>(SDLK_DISPLAYSWITCH));
	Nan::Set(KEY, Nan::New<String>("KBDILLUMTOGGLE"), Nan::New<Number>(SDLK_KBDILLUMTOGGLE));
	Nan::Set(KEY, Nan::New<String>("KBDILLUMDOWN"), Nan::New<Number>(SDLK_KBDILLUMDOWN));
	Nan::Set(KEY, Nan::New<String>("KBDILLUMUP"), Nan::New<Number>(SDLK_KBDILLUMUP));
	Nan::Set(KEY, Nan::New<String>("EJECT"), Nan::New<Number>(SDLK_EJECT));
	Nan::Set(KEY, Nan::New<String>("SLEEP"), Nan::New<Number>(SDLK_SLEEP));

	Handle<Object> SCANCODE = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("SCANCODE"), SCANCODE);
	// Keys are set in the order that they are listed on this page:
	//  http://wiki.libsdl.org/SDLScancodeLookup
	Nan::Set(SCANCODE, Nan::New<String>("UNKNOWN"), Nan::New<Number>(SDL_SCANCODE_UNKNOWN));
	Nan::Set(SCANCODE, Nan::New<String>("A"), Nan::New<Number>(SDL_SCANCODE_A));
	Nan::Set(SCANCODE, Nan::New<String>("B"), Nan::New<Number>(SDL_SCANCODE_B));
	Nan::Set(SCANCODE, Nan::New<String>("C"), Nan::New<Number>(SDL_SCANCODE_C));
	Nan::Set(SCANCODE, Nan::New<String>("D"), Nan::New<Number>(SDL_SCANCODE_D));
	Nan::Set(SCANCODE, Nan::New<String>("E"), Nan::New<Number>(SDL_SCANCODE_E));
	Nan::Set(SCANCODE, Nan::New<String>("F"), Nan::New<Number>(SDL_SCANCODE_F));
	Nan::Set(SCANCODE, Nan::New<String>("G"), Nan::New<Number>(SDL_SCANCODE_G));
	Nan::Set(SCANCODE, Nan::New<String>("H"), Nan::New<Number>(SDL_SCANCODE_H));
	Nan::Set(SCANCODE, Nan::New<String>("I"), Nan::New<Number>(SDL_SCANCODE_I));
	Nan::Set(SCANCODE, Nan::New<String>("J"), Nan::New<Number>(SDL_SCANCODE_J));
	Nan::Set(SCANCODE, Nan::New<String>("K"), Nan::New<Number>(SDL_SCANCODE_K));
	Nan::Set(SCANCODE, Nan::New<String>("L"), Nan::New<Number>(SDL_SCANCODE_L));
	Nan::Set(SCANCODE, Nan::New<String>("M"), Nan::New<Number>(SDL_SCANCODE_M));
	Nan::Set(SCANCODE, Nan::New<String>("N"), Nan::New<Number>(SDL_SCANCODE_N));
	Nan::Set(SCANCODE, Nan::New<String>("O"), Nan::New<Number>(SDL_SCANCODE_O));
	Nan::Set(SCANCODE, Nan::New<String>("P"), Nan::New<Number>(SDL_SCANCODE_P));
	Nan::Set(SCANCODE, Nan::New<String>("Q"), Nan::New<Number>(SDL_SCANCODE_Q));
	Nan::Set(SCANCODE, Nan::New<String>("R"), Nan::New<Number>(SDL_SCANCODE_R));
	Nan::Set(SCANCODE, Nan::New<String>("S"), Nan::New<Number>(SDL_SCANCODE_S));
	Nan::Set(SCANCODE, Nan::New<String>("T"), Nan::New<Number>(SDL_SCANCODE_T));
	Nan::Set(SCANCODE, Nan::New<String>("U"), Nan::New<Number>(SDL_SCANCODE_U));
	Nan::Set(SCANCODE, Nan::New<String>("V"), Nan::New<Number>(SDL_SCANCODE_V));
	Nan::Set(SCANCODE, Nan::New<String>("W"), Nan::New<Number>(SDL_SCANCODE_W));
	Nan::Set(SCANCODE, Nan::New<String>("X"), Nan::New<Number>(SDL_SCANCODE_X));
	Nan::Set(SCANCODE, Nan::New<String>("Y"), Nan::New<Number>(SDL_SCANCODE_Y));
	Nan::Set(SCANCODE, Nan::New<String>("ONE"), Nan::New<Number>(SDL_SCANCODE_1));
	Nan::Set(SCANCODE, Nan::New<String>("TWO"), Nan::New<Number>(SDL_SCANCODE_2));
	Nan::Set(SCANCODE, Nan::New<String>("THREE"), Nan::New<Number>(SDL_SCANCODE_3));
	Nan::Set(SCANCODE, Nan::New<String>("FOUR"), Nan::New<Number>(SDL_SCANCODE_4));
	Nan::Set(SCANCODE, Nan::New<String>("FIVE"), Nan::New<Number>(SDL_SCANCODE_5));
	Nan::Set(SCANCODE, Nan::New<String>("SIX"), Nan::New<Number>(SDL_SCANCODE_6));
	Nan::Set(SCANCODE, Nan::New<String>("SEVEN"), Nan::New<Number>(SDL_SCANCODE_7));
	Nan::Set(SCANCODE, Nan::New<String>("EIGHT"), Nan::New<Number>(SDL_SCANCODE_8));
	Nan::Set(SCANCODE, Nan::New<String>("NINE"), Nan::New<Number>(SDL_SCANCODE_9));
	Nan::Set(SCANCODE, Nan::New<String>("ZERO"), Nan::New<Number>(SDL_SCANCODE_0));
	Nan::Set(SCANCODE, Nan::New<String>("RETURN"), Nan::New<Number>(SDL_SCANCODE_RETURN));
	Nan::Set(SCANCODE, Nan::New<String>("BACKSPACE"), Nan::New<Number>(SDL_SCANCODE_BACKSPACE));
	Nan::Set(SCANCODE, Nan::New<String>("TAB"), Nan::New<Number>(SDL_SCANCODE_TAB));
	Nan::Set(SCANCODE, Nan::New<String>("SPACE"), Nan::New<Number>(SDL_SCANCODE_SPACE));
	Nan::Set(SCANCODE, Nan::New<String>("MINUS"), Nan::New<Number>(SDL_SCANCODE_MINUS));
	Nan::Set(SCANCODE, Nan::New<String>("EQUALS"), Nan::New<Number>(SDL_SCANCODE_EQUALS));
	Nan::Set(SCANCODE, Nan::New<String>("LEFTBRACKET"), Nan::New<Number>(SDL_SCANCODE_LEFTBRACKET));
	Nan::Set(SCANCODE, Nan::New<String>("RIGHTBRACKET"), Nan::New<Number>(SDL_SCANCODE_RIGHTBRACKET));
	Nan::Set(SCANCODE, Nan::New<String>("BACKSLASH"), Nan::New<Number>(SDL_SCANCODE_BACKSLASH));
	Nan::Set(SCANCODE, Nan::New<String>("NONUSHASH"), Nan::New<Number>(SDL_SCANCODE_NONUSHASH));
	Nan::Set(SCANCODE, Nan::New<String>("SEMICOLON"), Nan::New<Number>(SDL_SCANCODE_SEMICOLON));
	Nan::Set(SCANCODE, Nan::New<String>("APOSTROPHE"), Nan::New<Number>(SDL_SCANCODE_APOSTROPHE));
	Nan::Set(SCANCODE, Nan::New<String>("GRAVE"), Nan::New<Number>(SDL_SCANCODE_GRAVE));
	Nan::Set(SCANCODE, Nan::New<String>("COMMA"), Nan::New<Number>(SDL_SCANCODE_COMMA));
	Nan::Set(SCANCODE, Nan::New<String>("PERIOD"), Nan::New<Number>(SDL_SCANCODE_PERIOD));
	Nan::Set(SCANCODE, Nan::New<String>("SLASH"), Nan::New<Number>(SDL_SCANCODE_SLASH));
	Nan::Set(SCANCODE, Nan::New<String>("CAPSLOCK"), Nan::New<Number>(SDL_SCANCODE_CAPSLOCK));
	Nan::Set(SCANCODE, Nan::New<String>("F1"), Nan::New<Number>(SDL_SCANCODE_F1));
	Nan::Set(SCANCODE, Nan::New<String>("F2"), Nan::New<Number>(SDL_SCANCODE_F2));
	Nan::Set(SCANCODE, Nan::New<String>("F3"), Nan::New<Number>(SDL_SCANCODE_F3));
	Nan::Set(SCANCODE, Nan::New<String>("F4"), Nan::New<Number>(SDL_SCANCODE_F4));
	Nan::Set(SCANCODE, Nan::New<String>("F5"), Nan::New<Number>(SDL_SCANCODE_F5));
	Nan::Set(SCANCODE, Nan::New<String>("F6"), Nan::New<Number>(SDL_SCANCODE_F6));
	Nan::Set(SCANCODE, Nan::New<String>("F7"), Nan::New<Number>(SDL_SCANCODE_F7));
	Nan::Set(SCANCODE, Nan::New<String>("F8"), Nan::New<Number>(SDL_SCANCODE_F8));
	Nan::Set(SCANCODE, Nan::New<String>("F9"), Nan::New<Number>(SDL_SCANCODE_F9));
	Nan::Set(SCANCODE, Nan::New<String>("F10"), Nan::New<Number>(SDL_SCANCODE_F10));
	Nan::Set(SCANCODE, Nan::New<String>("F11"), Nan::New<Number>(SDL_SCANCODE_F11));
	Nan::Set(SCANCODE, Nan::New<String>("F12"), Nan::New<Number>(SDL_SCANCODE_F12));
	Nan::Set(SCANCODE, Nan::New<String>("PRINTSCREEN"), Nan::New<Number>(SDL_SCANCODE_PRINTSCREEN));
	Nan::Set(SCANCODE, Nan::New<String>("SCROLLLOCK"), Nan::New<Number>(SDL_SCANCODE_SCROLLLOCK));
	Nan::Set(SCANCODE, Nan::New<String>("PAUSE"), Nan::New<Number>(SDL_SCANCODE_PAUSE));
	Nan::Set(SCANCODE, Nan::New<String>("INSERT"), Nan::New<Number>(SDL_SCANCODE_INSERT));
	Nan::Set(SCANCODE, Nan::New<String>("HOME"), Nan::New<Number>(SDL_SCANCODE_HOME));
	Nan::Set(SCANCODE, Nan::New<String>("PAGEUP"), Nan::New<Number>(SDL_SCANCODE_PAGEUP));
	Nan::Set(SCANCODE, Nan::New<String>("DELETE"), Nan::New<Number>(SDL_SCANCODE_DELETE));
	Nan::Set(SCANCODE, Nan::New<String>("END"), Nan::New<Number>(SDL_SCANCODE_END));
	Nan::Set(SCANCODE, Nan::New<String>("PAGEDOWN"), Nan::New<Number>(SDL_SCANCODE_PAGEDOWN));
	Nan::Set(SCANCODE, Nan::New<String>("RIGHT"), Nan::New<Number>(SDL_SCANCODE_RIGHT));
	Nan::Set(SCANCODE, Nan::New<String>("LEFT"), Nan::New<Number>(SDL_SCANCODE_LEFT));
	Nan::Set(SCANCODE, Nan::New<String>("DOWN"), Nan::New<Number>(SDL_SCANCODE_DOWN));
	Nan::Set(SCANCODE, Nan::New<String>("UP"), Nan::New<Number>(SDL_SCANCODE_UP));
	Nan::Set(SCANCODE, Nan::New<String>("NUMLOCKCLEAR"), Nan::New<Number>(SDL_SCANCODE_NUMLOCKCLEAR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_DIVIDE"), Nan::New<Number>(SDL_SCANCODE_KP_DIVIDE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MULTIPLY"), Nan::New<Number>(SDL_SCANCODE_KP_MULTIPLY));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MINUS"), Nan::New<Number>(SDL_SCANCODE_KP_MINUS));
	Nan::Set(SCANCODE, Nan::New<String>("KP_PLUS"), Nan::New<Number>(SDL_SCANCODE_KP_ENTER));
	Nan::Set(SCANCODE, Nan::New<String>("KP_ONE"), Nan::New<Number>(SDL_SCANCODE_KP_1));
	Nan::Set(SCANCODE, Nan::New<String>("KP_TWO"), Nan::New<Number>(SDL_SCANCODE_KP_2));
	Nan::Set(SCANCODE, Nan::New<String>("KP_THREE"), Nan::New<Number>(SDL_SCANCODE_KP_3));
	Nan::Set(SCANCODE, Nan::New<String>("KP_FOUR"), Nan::New<Number>(SDL_SCANCODE_KP_4));
	Nan::Set(SCANCODE, Nan::New<String>("KP_FIVE"), Nan::New<Number>(SDL_SCANCODE_KP_5));
	Nan::Set(SCANCODE, Nan::New<String>("KP_SIX"), Nan::New<Number>(SDL_SCANCODE_KP_6));
	Nan::Set(SCANCODE, Nan::New<String>("KP_SEVEN"), Nan::New<Number>(SDL_SCANCODE_KP_7));
	Nan::Set(SCANCODE, Nan::New<String>("KP_EIGHT"), Nan::New<Number>(SDL_SCANCODE_KP_8));
	Nan::Set(SCANCODE, Nan::New<String>("KP_NINE"), Nan::New<Number>(SDL_SCANCODE_KP_9));
	Nan::Set(SCANCODE, Nan::New<String>("KP_ZERO"), Nan::New<Number>(SDL_SCANCODE_KP_0));
	Nan::Set(SCANCODE, Nan::New<String>("KP_PERIOD"), Nan::New<Number>(SDL_SCANCODE_KP_PERIOD));
	Nan::Set(SCANCODE, Nan::New<String>("NONUSHBACKSLASH"), Nan::New<Number>(SDL_SCANCODE_NONUSBACKSLASH));
	Nan::Set(SCANCODE, Nan::New<String>("APPLICATION"), Nan::New<Number>(SDL_SCANCODE_APPLICATION));
	Nan::Set(SCANCODE, Nan::New<String>("POWER"), Nan::New<Number>(SDL_SCANCODE_POWER));
	Nan::Set(SCANCODE, Nan::New<String>("KP_EQUALS"), Nan::New<Number>(SDL_SCANCODE_KP_EQUALS));
	Nan::Set(SCANCODE, Nan::New<String>("F13"), Nan::New<Number>(SDL_SCANCODE_F13));
	Nan::Set(SCANCODE, Nan::New<String>("F14"), Nan::New<Number>(SDL_SCANCODE_F14));
	Nan::Set(SCANCODE, Nan::New<String>("F15"), Nan::New<Number>(SDL_SCANCODE_F15));
	Nan::Set(SCANCODE, Nan::New<String>("F16"), Nan::New<Number>(SDL_SCANCODE_F16));
	Nan::Set(SCANCODE, Nan::New<String>("F17"), Nan::New<Number>(SDL_SCANCODE_F17));
	Nan::Set(SCANCODE, Nan::New<String>("F18"), Nan::New<Number>(SDL_SCANCODE_F18));
	Nan::Set(SCANCODE, Nan::New<String>("F19"), Nan::New<Number>(SDL_SCANCODE_F19));
	Nan::Set(SCANCODE, Nan::New<String>("F20"), Nan::New<Number>(SDL_SCANCODE_F20));
	Nan::Set(SCANCODE, Nan::New<String>("F21"), Nan::New<Number>(SDL_SCANCODE_F21));
	Nan::Set(SCANCODE, Nan::New<String>("F22"), Nan::New<Number>(SDL_SCANCODE_F22));
	Nan::Set(SCANCODE, Nan::New<String>("F23"), Nan::New<Number>(SDL_SCANCODE_F23));
	Nan::Set(SCANCODE, Nan::New<String>("F24"), Nan::New<Number>(SDL_SCANCODE_F24));
	Nan::Set(SCANCODE, Nan::New<String>("EXECUTE"), Nan::New<Number>(SDL_SCANCODE_EXECUTE));
	Nan::Set(SCANCODE, Nan::New<String>("HELP"), Nan::New<Number>(SDL_SCANCODE_HELP));
	Nan::Set(SCANCODE, Nan::New<String>("MENU"), Nan::New<Number>(SDL_SCANCODE_MENU));
	Nan::Set(SCANCODE, Nan::New<String>("SELECT"), Nan::New<Number>(SDL_SCANCODE_SELECT));
	Nan::Set(SCANCODE, Nan::New<String>("STOP"), Nan::New<Number>(SDL_SCANCODE_STOP));
	Nan::Set(SCANCODE, Nan::New<String>("AGAIN"), Nan::New<Number>(SDL_SCANCODE_AGAIN));
	Nan::Set(SCANCODE, Nan::New<String>("UNDO"), Nan::New<Number>(SDL_SCANCODE_UNDO));
	Nan::Set(SCANCODE, Nan::New<String>("CUT"), Nan::New<Number>(SDL_SCANCODE_CUT));
	Nan::Set(SCANCODE, Nan::New<String>("COPY"), Nan::New<Number>(SDL_SCANCODE_COPY));
	Nan::Set(SCANCODE, Nan::New<String>("PASTE"), Nan::New<Number>(SDL_SCANCODE_PASTE));
	Nan::Set(SCANCODE, Nan::New<String>("FIND"), Nan::New<Number>(SDL_SCANCODE_FIND));
	Nan::Set(SCANCODE, Nan::New<String>("MUTE"), Nan::New<Number>(SDL_SCANCODE_MUTE));
	Nan::Set(SCANCODE, Nan::New<String>("VOLUMEUP"), Nan::New<Number>(SDL_SCANCODE_VOLUMEUP));
	Nan::Set(SCANCODE, Nan::New<String>("VOLUMEDOWN"), Nan::New<Number>(SDL_SCANCODE_VOLUMEDOWN));
	// For some reason, these are defined in the lookup table but don't exist in a header?
	// Nan::Set(SCANCODE, Nan::New<String>("LOCKINGCAPSLOCK"), Nan::New<Number>(SDL_SCANCODE_LOCKINGCAPSLOCK));
	// Nan::Set(SCANCODE, Nan::New<String>("LOCKINGNUMLOCK"), Nan::New<Number>(SDL_SCANCODE_LOCKINGNUMLOCK));
	// Nan::Set(SCANCODE, Nan::New<String>("LOCKINGSCROLLLOCK"), Nan::New<Number>(SDL_SCANCODE_LOCKINGSCROLLLOCK));
	Nan::Set(SCANCODE, Nan::New<String>("KP_COMMA"), Nan::New<Number>(SDL_SCANCODE_KP_COMMA));
	Nan::Set(SCANCODE, Nan::New<String>("KP_EQUALSAS400"), Nan::New<Number>(SDL_SCANCODE_KP_EQUALSAS400));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL1"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL1));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL2"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL2));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL3"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL3));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL4"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL4));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL5"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL5));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL6"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL6));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL7"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL7));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL8"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL8));
	Nan::Set(SCANCODE, Nan::New<String>("INTERNATIONAL9"), Nan::New<Number>(SDL_SCANCODE_INTERNATIONAL9));
	Nan::Set(SCANCODE, Nan::New<String>("LANG1"), Nan::New<Number>(SDL_SCANCODE_LANG1));
	Nan::Set(SCANCODE, Nan::New<String>("LANG2"), Nan::New<Number>(SDL_SCANCODE_LANG2));
	Nan::Set(SCANCODE, Nan::New<String>("LANG3"), Nan::New<Number>(SDL_SCANCODE_LANG3));
	Nan::Set(SCANCODE, Nan::New<String>("LANG4"), Nan::New<Number>(SDL_SCANCODE_LANG4));
	Nan::Set(SCANCODE, Nan::New<String>("LANG5"), Nan::New<Number>(SDL_SCANCODE_LANG5));
	Nan::Set(SCANCODE, Nan::New<String>("LANG6"), Nan::New<Number>(SDL_SCANCODE_LANG6));
	Nan::Set(SCANCODE, Nan::New<String>("LANG7"), Nan::New<Number>(SDL_SCANCODE_LANG7));
	Nan::Set(SCANCODE, Nan::New<String>("LANG8"), Nan::New<Number>(SDL_SCANCODE_LANG8));
	Nan::Set(SCANCODE, Nan::New<String>("LANG9"), Nan::New<Number>(SDL_SCANCODE_LANG9));
	Nan::Set(SCANCODE, Nan::New<String>("ALTERASE"), Nan::New<Number>(SDL_SCANCODE_ALTERASE));
	Nan::Set(SCANCODE, Nan::New<String>("SYSREQ"), Nan::New<Number>(SDL_SCANCODE_SYSREQ));
	Nan::Set(SCANCODE, Nan::New<String>("CANCEL"), Nan::New<Number>(SDL_SCANCODE_CANCEL));
	Nan::Set(SCANCODE, Nan::New<String>("CLEAR"), Nan::New<Number>(SDL_SCANCODE_CLEAR));
	Nan::Set(SCANCODE, Nan::New<String>("PRIOR"), Nan::New<Number>(SDL_SCANCODE_PRIOR));
	Nan::Set(SCANCODE, Nan::New<String>("RETURN2"), Nan::New<Number>(SDL_SCANCODE_RETURN2));
	Nan::Set(SCANCODE, Nan::New<String>("SEPARATOR"), Nan::New<Number>(SDL_SCANCODE_SEPARATOR));
	Nan::Set(SCANCODE, Nan::New<String>("OUT"), Nan::New<Number>(SDL_SCANCODE_OUT));
	Nan::Set(SCANCODE, Nan::New<String>("OPER"), Nan::New<Number>(SDL_SCANCODE_OPER));
	Nan::Set(SCANCODE, Nan::New<String>("CLEARAGAIN"), Nan::New<Number>(SDL_SCANCODE_CLEARAGAIN));
	Nan::Set(SCANCODE, Nan::New<String>("CRSEL"), Nan::New<Number>(SDL_SCANCODE_CRSEL));
	Nan::Set(SCANCODE, Nan::New<String>("EXSEL"), Nan::New<Number>(SDL_SCANCODE_EXSEL));
	Nan::Set(SCANCODE, Nan::New<String>("KP_ZEROZERO"), Nan::New<Number>(SDL_SCANCODE_KP_00));
	Nan::Set(SCANCODE, Nan::New<String>("KP_ZEROZEROZERO"), Nan::New<Number>(SDL_SCANCODE_KP_000));
	Nan::Set(SCANCODE, Nan::New<String>("THOUSANDSSEPARATOR"), Nan::New<Number>(SDL_SCANCODE_THOUSANDSSEPARATOR));
	Nan::Set(SCANCODE, Nan::New<String>("DECIMALSEPARATOR"), Nan::New<Number>(SDL_SCANCODE_DECIMALSEPARATOR));
	Nan::Set(SCANCODE, Nan::New<String>("CURRENCYUNIT"), Nan::New<Number>(SDL_SCANCODE_CURRENCYUNIT));
	Nan::Set(SCANCODE, Nan::New<String>("CURRENCYSUBUNIT"), Nan::New<Number>(SDL_SCANCODE_CURRENCYSUBUNIT));
	Nan::Set(SCANCODE, Nan::New<String>("KP_LEFTPAREN"), Nan::New<Number>(SDL_SCANCODE_KP_LEFTPAREN));
	Nan::Set(SCANCODE, Nan::New<String>("KP_RIGHTPAREN"), Nan::New<Number>(SDL_SCANCODE_KP_RIGHTPAREN));
	Nan::Set(SCANCODE, Nan::New<String>("KP_LEFTBRACE"), Nan::New<Number>(SDL_SCANCODE_KP_LEFTBRACE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_RIGHTBRACE"), Nan::New<Number>(SDL_SCANCODE_KP_RIGHTBRACE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_TAB"), Nan::New<Number>(SDL_SCANCODE_KP_TAB));
	Nan::Set(SCANCODE, Nan::New<String>("KP_BACKSPACE"), Nan::New<Number>(SDL_SCANCODE_KP_BACKSPACE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_A"), Nan::New<Number>(SDL_SCANCODE_KP_A));
	Nan::Set(SCANCODE, Nan::New<String>("KP_B"), Nan::New<Number>(SDL_SCANCODE_KP_D));
	Nan::Set(SCANCODE, Nan::New<String>("KP_C"), Nan::New<Number>(SDL_SCANCODE_KP_C));
	Nan::Set(SCANCODE, Nan::New<String>("KP_D"), Nan::New<Number>(SDL_SCANCODE_KP_D));
	Nan::Set(SCANCODE, Nan::New<String>("KP_E"), Nan::New<Number>(SDL_SCANCODE_KP_E));
	Nan::Set(SCANCODE, Nan::New<String>("KP_F"), Nan::New<Number>(SDL_SCANCODE_KP_F));
	Nan::Set(SCANCODE, Nan::New<String>("KP_XOR"), Nan::New<Number>(SDL_SCANCODE_KP_XOR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_POWER"), Nan::New<Number>(SDL_SCANCODE_KP_POWER));
	Nan::Set(SCANCODE, Nan::New<String>("KP_PERCENT"), Nan::New<Number>(SDL_SCANCODE_KP_PERCENT));
	Nan::Set(SCANCODE, Nan::New<String>("KP_LESS"), Nan::New<Number>(SDL_SCANCODE_KP_LESS));
	Nan::Set(SCANCODE, Nan::New<String>("KP_GREATER"), Nan::New<Number>(SDL_SCANCODE_KP_GREATER));
	Nan::Set(SCANCODE, Nan::New<String>("KP_AMPERSAND"), Nan::New<Number>(SDL_SCANCODE_KP_AMPERSAND));
	Nan::Set(SCANCODE, Nan::New<String>("KP_DBLAMPERSAND"), Nan::New<Number>(SDL_SCANCODE_KP_DBLAMPERSAND));
	Nan::Set(SCANCODE, Nan::New<String>("KP_VERTICALBAR"), Nan::New<Number>(SDL_SCANCODE_KP_VERTICALBAR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_DBLVERTICALBAR"), Nan::New<Number>(SDL_SCANCODE_KP_DBLVERTICALBAR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_COLON"), Nan::New<Number>(SDL_SCANCODE_KP_COLON));
	Nan::Set(SCANCODE, Nan::New<String>("KP_HASH"), Nan::New<Number>(SDL_SCANCODE_KP_HASH));
	Nan::Set(SCANCODE, Nan::New<String>("KP_SPACE"), Nan::New<Number>(SDL_SCANCODE_KP_SPACE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_AT"), Nan::New<Number>(SDL_SCANCODE_KP_AT));
	Nan::Set(SCANCODE, Nan::New<String>("KP_EXCLAM"), Nan::New<Number>(SDL_SCANCODE_KP_EXCLAM));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMSTORE"), Nan::New<Number>(SDL_SCANCODE_KP_MEMSTORE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMRECALL"), Nan::New<Number>(SDL_SCANCODE_KP_MEMRECALL));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMCLEAR"), Nan::New<Number>(SDL_SCANCODE_KP_MEMCLEAR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMADD"), Nan::New<Number>(SDL_SCANCODE_KP_MEMADD));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMSUBTRACT"), Nan::New<Number>(SDL_SCANCODE_KP_MEMSUBTRACT));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMMULTIPLY"), Nan::New<Number>(SDL_SCANCODE_KP_MEMMULTIPLY));
	Nan::Set(SCANCODE, Nan::New<String>("KP_MEMDIVIDE"), Nan::New<Number>(SDL_SCANCODE_KP_MEMDIVIDE));
	Nan::Set(SCANCODE, Nan::New<String>("KP_PLUSMINUS"), Nan::New<Number>(SDL_SCANCODE_KP_PLUSMINUS));
	Nan::Set(SCANCODE, Nan::New<String>("KP_CLEAR"), Nan::New<Number>(SDL_SCANCODE_KP_CLEAR));
	Nan::Set(SCANCODE, Nan::New<String>("KP_CLEARENTRY"), Nan::New<Number>(SDL_SCANCODE_KP_CLEARENTRY));
	Nan::Set(SCANCODE, Nan::New<String>("KP_BINARY"), Nan::New<Number>(SDL_SCANCODE_KP_BINARY));
	Nan::Set(SCANCODE, Nan::New<String>("KP_OCTAL"), Nan::New<Number>(SDL_SCANCODE_KP_OCTAL));
	Nan::Set(SCANCODE, Nan::New<String>("KP_DECIMAL"), Nan::New<Number>(SDL_SCANCODE_KP_DECIMAL));
	Nan::Set(SCANCODE, Nan::New<String>("KP_HEXADECIMAL"), Nan::New<Number>(SDL_SCANCODE_KP_HEXADECIMAL));
	Nan::Set(SCANCODE, Nan::New<String>("LCTRL"), Nan::New<Number>(SDL_SCANCODE_LCTRL));
	Nan::Set(SCANCODE, Nan::New<String>("LSHIFT"), Nan::New<Number>(SDL_SCANCODE_LSHIFT));
	Nan::Set(SCANCODE, Nan::New<String>("LALT"), Nan::New<Number>(SDL_SCANCODE_LALT));
	Nan::Set(SCANCODE, Nan::New<String>("LGUI"), Nan::New<Number>(SDL_SCANCODE_LGUI));
	Nan::Set(SCANCODE, Nan::New<String>("RCTRL"), Nan::New<Number>(SDL_SCANCODE_RCTRL));
	Nan::Set(SCANCODE, Nan::New<String>("RSHIFT"), Nan::New<Number>(SDL_SCANCODE_RSHIFT));
	Nan::Set(SCANCODE, Nan::New<String>("RALT"), Nan::New<Number>(SDL_SCANCODE_RALT));
	Nan::Set(SCANCODE, Nan::New<String>("RGUI"), Nan::New<Number>(SDL_SCANCODE_RGUI));
	Nan::Set(SCANCODE, Nan::New<String>("MODE"), Nan::New<Number>(SDL_SCANCODE_MODE));
	Nan::Set(SCANCODE, Nan::New<String>("AUDIONEXT"), Nan::New<Number>(SDL_SCANCODE_AUDIONEXT));
	Nan::Set(SCANCODE, Nan::New<String>("AUDIOPREV"), Nan::New<Number>(SDL_SCANCODE_AUDIOPREV));
	Nan::Set(SCANCODE, Nan::New<String>("AUDIOSTOP"), Nan::New<Number>(SDL_SCANCODE_AUDIOSTOP));
	Nan::Set(SCANCODE, Nan::New<String>("AUDIOPLAY"), Nan::New<Number>(SDL_SCANCODE_AUDIOPLAY));
	Nan::Set(SCANCODE, Nan::New<String>("AUDIOMUTE"), Nan::New<Number>(SDL_SCANCODE_AUDIOMUTE));
	Nan::Set(SCANCODE, Nan::New<String>("MEDIASELECT"), Nan::New<Number>(SDL_SCANCODE_MEDIASELECT));
	Nan::Set(SCANCODE, Nan::New<String>("WWW"), Nan::New<Number>(SDL_SCANCODE_WWW));
	Nan::Set(SCANCODE, Nan::New<String>("MAIL"), Nan::New<Number>(SDL_SCANCODE_MAIL));
	Nan::Set(SCANCODE, Nan::New<String>("CALCULATOR"), Nan::New<Number>(SDL_SCANCODE_CALCULATOR));
	Nan::Set(SCANCODE, Nan::New<String>("COMPUTER"), Nan::New<Number>(SDL_SCANCODE_COMPUTER));
	Nan::Set(SCANCODE, Nan::New<String>("AC_SEARCH"), Nan::New<Number>(SDL_SCANCODE_AC_SEARCH));
	Nan::Set(SCANCODE, Nan::New<String>("AC_HOME"), Nan::New<Number>(SDL_SCANCODE_AC_HOME));
	Nan::Set(SCANCODE, Nan::New<String>("AC_BACK"), Nan::New<Number>(SDL_SCANCODE_AC_BACK));
	Nan::Set(SCANCODE, Nan::New<String>("AC_FORWARD"), Nan::New<Number>(SDL_SCANCODE_AC_FORWARD));
	Nan::Set(SCANCODE, Nan::New<String>("AC_STOP"), Nan::New<Number>(SDL_SCANCODE_AC_STOP));
	Nan::Set(SCANCODE, Nan::New<String>("AC_REFRESH"), Nan::New<Number>(SDL_SCANCODE_AC_REFRESH));
	Nan::Set(SCANCODE, Nan::New<String>("AC_BOOKMARKS"), Nan::New<Number>(SDL_SCANCODE_AC_BOOKMARKS));
	Nan::Set(SCANCODE, Nan::New<String>("BRIGHTNESSDOWN"), Nan::New<Number>(SDL_SCANCODE_BRIGHTNESSDOWN));
	Nan::Set(SCANCODE, Nan::New<String>("BRIGHTNESSUP"), Nan::New<Number>(SDL_SCANCODE_BRIGHTNESSUP));
	Nan::Set(SCANCODE, Nan::New<String>("DISPLAYSWITCH"), Nan::New<Number>(SDL_SCANCODE_DISPLAYSWITCH));
	Nan::Set(SCANCODE, Nan::New<String>("KBDILLUMTOGGLE"), Nan::New<Number>(SDL_SCANCODE_KBDILLUMTOGGLE));
	Nan::Set(SCANCODE, Nan::New<String>("KBDILLUMDOWN"), Nan::New<Number>(SDL_SCANCODE_KBDILLUMDOWN));
	Nan::Set(SCANCODE, Nan::New<String>("KBDILLUMUP"), Nan::New<Number>(SDL_SCANCODE_KBDILLUMUP));
	Nan::Set(SCANCODE, Nan::New<String>("EJECT"), Nan::New<Number>(SDL_SCANCODE_EJECT));
	Nan::Set(SCANCODE, Nan::New<String>("SLEEP"), Nan::New<Number>(SDL_SCANCODE_SLEEP));

	Handle<Object> KMOD = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("KMOD"), KMOD);
	Nan::Set(KMOD, Nan::New<String>("NONE"), Nan::New<Number>(KMOD_NONE));
	Nan::Set(KMOD, Nan::New<String>("LSHIFT"), Nan::New<Number>(KMOD_LSHIFT));
	Nan::Set(KMOD, Nan::New<String>("RSHIFT"), Nan::New<Number>(KMOD_RSHIFT));
	Nan::Set(KMOD, Nan::New<String>("LCTRL"), Nan::New<Number>(KMOD_LCTRL));
	Nan::Set(KMOD, Nan::New<String>("RCTRL"), Nan::New<Number>(KMOD_RCTRL));
	Nan::Set(KMOD, Nan::New<String>("LALT"), Nan::New<Number>(KMOD_LALT));
	Nan::Set(KMOD, Nan::New<String>("RALT"), Nan::New<Number>(KMOD_RALT));
	Nan::Set(KMOD, Nan::New<String>("LGUI"), Nan::New<Number>(KMOD_LGUI));
	Nan::Set(KMOD, Nan::New<String>("RGUI"), Nan::New<Number>(KMOD_RGUI));
	Nan::Set(KMOD, Nan::New<String>("NUM"), Nan::New<Number>(KMOD_NUM));
	Nan::Set(KMOD, Nan::New<String>("CAPS"), Nan::New<Number>(KMOD_CAPS));
	Nan::Set(KMOD, Nan::New<String>("MODE"), Nan::New<Number>(KMOD_MODE));
	Nan::Set(KMOD, Nan::New<String>("CTRL"), Nan::New<Number>(KMOD_CTRL));
	Nan::Set(KMOD, Nan::New<String>("SHIFT"), Nan::New<Number>(KMOD_SHIFT));
	Nan::Set(KMOD, Nan::New<String>("ALT"), Nan::New<Number>(KMOD_ALT));
	Nan::Set(KMOD, Nan::New<String>("GUI"), Nan::New<Number>(KMOD_GUI));

	NODE_SET_METHOD(exports, "getKeyFromName", GetKeyFromName);
	NODE_SET_METHOD(exports, "getKeyFromScancode", GetKeyFromScancode);
	NODE_SET_METHOD(exports, "getKeyName", GetKeyName);

	NODE_SET_METHOD(exports, "getScancodeFromKey", GetScancodeFromKey);
	NODE_SET_METHOD(exports, "getScancodeFromName", GetScancodeFromName);
	NODE_SET_METHOD(exports, "getScancodeName", GetScancodeName);

	NODE_SET_METHOD(exports, "hasScreenKeyboardSupport", HasScreenKeyboardSupport);
	NODE_SET_METHOD(exports, "isScreenKeyboardShown", IsScreenKeyboardShown);

	NODE_SET_METHOD(exports, "getKeyboardFocus", GetKeyboardFocus);
	NODE_SET_METHOD(exports, "getKeyboardState", GetKeyboardState);
	NODE_SET_METHOD(exports, "getModState", GetModState);
	NODE_SET_METHOD(exports, "setModState", SetModState);

	NODE_SET_METHOD(exports, "isTextInputActive", IsTextInputActive);
	NODE_SET_METHOD(exports, "setTextInputRect", SetTextInputRect);
	NODE_SET_METHOD(exports, "startTextInput", StartTextInput);
	NODE_SET_METHOD(exports, "stopTextInput", StopTextInput);
}

// NAN_METHOD(sdl::GetKeyFromName) {
// 	HandleScope scope;

// 	if(args.Length() < 1) {
// 		return ThrowException(Exception::TypeError(
// 			Nan::New<String>("Invalid arguments: Expected GetKeyFromName(String)")));
// 	}

// 	String::Utf8Value name(args[0]);
// 	SDL_Keycode code = SDL_GetKeyFromName(*name);

// 	return scope.Close(Nan::New<Number>(code));
// }
FUNCTION_BEGINP(sdl, GetKeyFromName, 1)
	EXTRACT_STRING(name, 0);
	SDL_Keycode code = SDL_GetKeyFromName(*name);
FUNCTION_END(Nan::New<Number>(code))

// NAN_METHOD(sdl::GetKeyFromScancode) {
// 	HandleScope scope;

// 	if(args.Length() < 1) {
// 		return ThrowException(Exception::TypeError(
// 			Nan::New<String>("Invalid arguments: Expected GetKeyFromScancode(Number)")));
// 	}

// 	SDL_Scancode scan = static_cast<SDL_Scancode>(args[0]->Int32Value());
// 	SDL_Keycode key = SDL_GetKeyFromScancode(scan);

// 	return scope.Close(Nan::New<Number>(key));
// }
FUNCTION_BEGINP(sdl, GetKeyFromScancode, 1)
	EXTRACT_INT32(scan, 0);
	SDL_Keycode key = SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(scan));
FUNCTION_END(Nan::New<Number>(key))

NAN_METHOD(sdl::GetKeyName) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetKeyName(Number)")));
	}

	SDL_Keycode key = static_cast<SDL_Keycode>(args[0]->Int32Value());
	const char *name = SDL_GetKeyName(key);

	return scope.Close(Nan::New<String>(name));
}

NAN_METHOD(sdl::GetScancodeFromKey) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetScancodeFromKey(Number)")));
	}

	SDL_Keycode key = static_cast<SDL_Keycode>(args[0]->Int32Value());
	SDL_Scancode scan = SDL_GetScancodeFromKey(key);

	return scope.Close(Nan::New<Number>(scan));
}
NAN_METHOD(sdl::GetScancodeFromName) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetScancodeFromName(String)")));
	}

	String::Utf8Value name(args[0]);
	SDL_Scancode scan = SDL_GetScancodeFromName(*name);

	return scope.Close(Nan::New<Number>(scan));
}
NAN_METHOD(sdl::GetScancodeName) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetScancodeName(Number)")));
	}

	SDL_Scancode scan = static_cast<SDL_Scancode>(args[0]->Int32Value());
	const char *name = SDL_GetScancodeName(scan);

	return scope.Close(Nan::New<String>(name));
}

NAN_METHOD(sdl::HasScreenKeyboardSupport) {
	HandleScope scope;

	SDL_bool ret = SDL_HasScreenKeyboardSupport();

	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}
NAN_METHOD(sdl::IsScreenKeyboardShown) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected IsScreenKeyboardShown(Window)")));
	}

	WindowWrapper* wrap = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	SDL_bool ret = SDL_IsScreenKeyboardShown(wrap->window_);

	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}

NAN_METHOD(sdl::GetKeyboardFocus) {
	HandleScope scope;

	SDL_Window* window = SDL_GetKeyboardFocus();
	Handle<Object> ret = Nan::New<Object>();
	WindowWrapper* wrap = new WindowWrapper(ret);
	wrap->window_ = window;

	return scope.Close(ret);
}
NAN_METHOD(sdl::GetKeyboardState) {
	HandleScope scope;

	int size;
	const Uint8 *state = SDL_GetKeyboardState(&size);

	// Copy the state of every key into a native Javascript array. The state returned
	// by SDL is to internal memory, therefore we don't want to manage it at all inside
	// of Javscript due to it possibly being freed at some point.
	//
	// SDL uses the value 1 and 0 to indicate pressed or not. This is translated to true
	// or false Javascript boolean values instead.
	Handle<Array> jsState = Array::New(size);
	for(int i = 0; i < size; i++) {
		Nan::Set(jsState, i, Nan::New<Boolean>(state[i] == 1 ? true : false));
	}

	return scope.Close(jsState);
}
NAN_METHOD(sdl::GetModState) {
	HandleScope scope;

	SDL_Keymod mod = SDL_GetModState();

	return scope.Close(Nan::New<Number>(mod));
}
NAN_METHOD(sdl::SetModState) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected SetModState(Number)")));
	}

	SDL_Keymod mod = static_cast<SDL_Keymod>(args[0]->Int32Value());
	SDL_SetModState(mod);

	return Undefined();
}

NAN_METHOD(sdl::IsTextInputActive) {
	HandleScope scope;

	SDL_bool ret = SDL_IsTextInputActive();

	return scope.Close(Nan::New<Boolean>(ret));
}
NAN_METHOD(sdl::SetTextInputRect) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected SetTextInputRect(Rect)")));
	}

	RectWrapper* rect = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[0]));
	SDL_SetTextInputRect(rect->wrapped);

	return Undefined();
}
NAN_METHOD(sdl::StartTextInput) {
	HandleScope scope;

	SDL_StartTextInput();

	return Undefined();
}
NAN_METHOD(sdl::StopTextInput) {
	HandleScope scope;

	SDL_StopTextInput();

	return Undefined();
}
