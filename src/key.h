#ifndef NODEJS_KEY_H
#define NODEJS_KEY_H

#include <v8.h>
#include <node.h>

#include "SDL.h"
#include "helpers.h"


namespace sdl {
	namespace key {
		void Init(v8::Handle<v8::Object> exports);
	}

	// NAN_METHOD(GetKeyFromName);
	FUNCTION_DEF(GetKeyFromName);
	NAN_METHOD(GetKeyFromScancode);
	NAN_METHOD(GetKeyName);
	
	NAN_METHOD(GetScancodeFromKey);
	NAN_METHOD(GetScancodeFromName);
	NAN_METHOD(GetScancodeName);

	NAN_METHOD(HasScreenKeyboardSupport);
	NAN_METHOD(IsScreenKeyboardShown);

	NAN_METHOD(GetKeyboardFocus);
	NAN_METHOD(GetKeyboardState);
	NAN_METHOD(GetModState);
	NAN_METHOD(SetModState);

	NAN_METHOD(IsTextInputActive);
	NAN_METHOD(SetTextInputRect);
	NAN_METHOD(StartTextInput);
	NAN_METHOD(StopTextInput);
}

#endif