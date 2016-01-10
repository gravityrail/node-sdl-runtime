#ifndef NODESDL_CONTROLLER_H
#define NODESDL_CONTROLLER_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace controller {
		void Init(v8::Handle<v8::Object> exports);

		class GameControllerWrapper : public Nan::ObjectWrap {
		public:
			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			GameControllerWrapper();
			~GameControllerWrapper();

			static void Init(v8::Handle<v8::Object> exports);
			static NAN_METHOD(New);

			static NAN_METHOD(GetAttached);
			static NAN_METHOD(GetAxis);
			static NAN_METHOD(GetBindForAxis);
			static NAN_METHOD(GetBindForButton);
			static NAN_METHOD(GetButton);
			static NAN_METHOD(GetJoystick);
			static NAN_METHOD(GetMapping);
			static NAN_METHOD(GetName);

			SDL_GameController* controller_;
		};

		NAN_METHOD(AddMapping);
		NAN_METHOD(AddMappingsFromFile);

		NAN_METHOD(MappingForGUID);
		NAN_METHOD(NameForIndex);

		NAN_METHOD(EventState);
		NAN_METHOD(GetAxisFromString);
		NAN_METHOD(GetButtonFromString);
		NAN_METHOD(GetStringForAxis);
		NAN_METHOD(GetStringForButton);

		NAN_METHOD(Update);
		NAN_METHOD(IsController);
	}
}

#endif
