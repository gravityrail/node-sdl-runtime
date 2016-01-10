#ifndef NODESDL_SDLGL_H
#define NODESDL_SDLGL_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace gl {
		void Init(v8::Handle<v8::Object> exports);

		class ContextWrapper : node::ObjectWrap {
		public:
			ContextWrapper();
			ContextWrapper(v8::Handle<v8::Object> obj);
			~ContextWrapper();

			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			static void Init(v8::Handle<v8::Object> exports);
			static NAN_METHOD(New);

			SDL_GLContext context_;
		};

		NAN_METHOD(BindTexture);
		NAN_METHOD(UnbindTexture);

		NAN_METHOD(ExtensionSupported);

		NAN_METHOD(LoadLibrary);
		NAN_METHOD(UnloadLibrary);

		NAN_METHOD(SetAttribute);
		NAN_METHOD(MakeCurrent);
		NAN_METHOD(SetSwapInterval);

		NAN_METHOD(GetAttribute);
		NAN_METHOD(GetCurrentContext);
		NAN_METHOD(GetCurrentWindow);
		NAN_METHOD(GetDrawableSize);
		NAN_METHOD(GetSwapInterval);
	}
}

#endif