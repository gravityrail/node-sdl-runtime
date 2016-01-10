#ifndef NODESDL_JOYSTICK_H
#define NODESDL_JOYSTICK_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace joystick {
		void Init(v8::Handle<v8::Object> exports);
	}

	class JoystickWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		JoystickWrapper();
		JoystickWrapper(bool owned);
		~JoystickWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static NAN_METHOD(New);

		static NAN_METHOD(GetAttached);
		static NAN_METHOD(GetAxis);
		static NAN_METHOD(GetBall);
		static NAN_METHOD(GetButton);
		static NAN_METHOD(GetGUID);
		static NAN_METHOD(GetHat);
		static NAN_METHOD(GetName);
		static NAN_METHOD(GetNumAxes);
		static NAN_METHOD(GetNumButtons);
		static NAN_METHOD(GetNumBalls);
		static NAN_METHOD(GetNumHats);

		SDL_Joystick* joystick_;
		bool owned_;
	};

	NAN_METHOD(NumJoysticks);

	NAN_METHOD(JoystickNameForIndex);
	NAN_METHOD(JoystickGetDeviceGUID);
	NAN_METHOD(JoystickGetGUIDFromString);
	NAN_METHOD(JoystickGetGUIDString);

	NAN_METHOD(JoystickUpdate);
	NAN_METHOD(JoystickEventState);
}

#endif