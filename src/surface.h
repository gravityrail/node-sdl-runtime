#ifndef NODESDL_SURFACE_H
#define NODESDL_SURFACE_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class SurfaceWrapper : public Nan::ObjectWrap {
	public:
	    static v8::Persistent<v8::FunctionTemplate> wrap_template_;

	    SurfaceWrapper();
	    SurfaceWrapper(v8::Handle<v8::Object> toWrap);
	    ~SurfaceWrapper();

	    static void Init(v8::Handle<v8::Object> exports);
	    static NAN_METHOD(New);

	    static NAN_METHOD(LoadBMP);
	    static NAN_METHOD(LoadBMPRW);
	    static NAN_METHOD(SaveBMP);
	    static NAN_METHOD(SaveBMPRW);

	    static NAN_METHOD(BlitScaled);
	    static NAN_METHOD(BlitSurface);
	    static NAN_METHOD(LowerBlit);
	    static NAN_METHOD(LowerBlitScaled);
	    static NAN_METHOD(ConvertSurface);
	    static NAN_METHOD(ConvertSurfaceFormat);

	    static NAN_METHOD(FillRect);
	    static NAN_METHOD(FillRects);

	    static NAN_METHOD(GetClipRect);
	    static NAN_METHOD(GetColorKey);
	    static NAN_METHOD(GetAlphaMod);
	    static NAN_METHOD(GetBlendMode);
	    static NAN_METHOD(GetColorMod);
	    static NAN_METHOD(GetWidth);
	    static NAN_METHOD(GetHeight);
	    static NAN_METHOD(GetPitch);
	    static NAN_METHOD(GetPixelFormat);

	    static NAN_METHOD(SetClipRect);
	    static NAN_METHOD(SetColorKey);
	    static NAN_METHOD(SetAlphaMod);
	    static NAN_METHOD(SetBlendMode);
	    static NAN_METHOD(SetColorMod);
	    static NAN_METHOD(SetPalette);
	    static NAN_METHOD(SetRLE);

	    static NAN_METHOD(Lock);
	    static NAN_METHOD(Unlock);
	    static NAN_METHOD(MustLock);

		SDL_Surface* surface_;
	};
}

#endif
