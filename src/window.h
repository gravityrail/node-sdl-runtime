#ifndef NODESDL_WINDOW_H
#define NODESDL_WINDOW_H

#include <node.h>
#include <v8.h>

#include "SDL.h"

namespace sdl {
	class WindowWrapper : public Nan::ObjectWrap {
	public:
		WindowWrapper();
		WindowWrapper(v8::Handle<v8::Object> obj);
		~WindowWrapper();

		static v8::Persistent<v8::FunctionTemplate> window_wrap_template_;

		static void Init(v8::Handle<v8::Object> exports);
		static NAN_METHOD(New);

		static NAN_METHOD(GetBrightness);
		static NAN_METHOD(GetDisplayIndex);
		static NAN_METHOD(GetDisplayMode);
		static NAN_METHOD(GetFlags);
		static NAN_METHOD(GetGammaRamp);
		static NAN_METHOD(GetGrab);
		static NAN_METHOD(GetWindowID);
		static NAN_METHOD(GetMaximumSize);
		static NAN_METHOD(GetMinimumSize);
		static NAN_METHOD(GetPixelFormat);
		static NAN_METHOD(GetPosition);
		static NAN_METHOD(GetSize);
		static NAN_METHOD(GetSurface);
		static NAN_METHOD(GetTitle);
	    // TODO: Implement this. Probably won't get to this anytime soon.
	    //       Too specific to be useful?
	    // static NAN_METHOD(GetWMInfo);

		static NAN_METHOD(Hide);
		static NAN_METHOD(Show);

		static NAN_METHOD(Maximize);
		static NAN_METHOD(Minimize);

		static NAN_METHOD(Raise);
		static NAN_METHOD(Restore);

		static NAN_METHOD(SetBordered);
		static NAN_METHOD(SetBrightness);
	    // TODO: Implement this? Because Javascript can just set arbritary data, not needed?
	    // static NAN_METHOD(SetData);
		static NAN_METHOD(SetDisplayMode);
		static NAN_METHOD(SetFullscreen);
		static NAN_METHOD(SetGammaRamp);
		static NAN_METHOD(SetGrab);
		static NAN_METHOD(SetIcon);
		static NAN_METHOD(SetMaximumSize);
		static NAN_METHOD(SetMinimumSize);
		static NAN_METHOD(SetPosition);
		static NAN_METHOD(SetSize);
		static NAN_METHOD(SetTitle);

		static NAN_METHOD(UpdateWindowSurface);
		static NAN_METHOD(UpdateWindowSurfaceRects);

		static NAN_METHOD(Swap);

		SDL_Window* window_;
	};
}

#endif
