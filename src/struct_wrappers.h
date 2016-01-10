#ifndef STRUCT_WRAPPERS_H
#define STRUCT_WRAPPERS_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <v8.h>
#include <nan.h>
#include <node.h>
#include <node_buffer.h>

namespace sdl {
	void InitWrappers(v8::Handle<v8::Object> exports);

	///////////////////////////////////////////////////////////////////////////////
  	// Wrappers and Unwrappers.
  	// These include v8::ObjectTemplates which have properties for the same fields
  	// as defined in the SDL documentation. (excluding anything SDL uses internally
  	// or is just plain unused)


	///////////////////////////////////////////////////////////////////////////////
	// SDL_Point Wrapper/Unwrapper.
	class PointWrapper : public Nan::ObjectWrap {
	public:
	    static v8::Persistent<v8::FunctionTemplate> point_wrap_template_;

		PointWrapper();
		~PointWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		// static NAN_METHOD(New);
		// static v8::Handle<v8::Value> GetX(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		// static v8::Handle<v8::Value> GetY(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		// static void SetX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		// static void SetY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		// static NAN_METHOD(ToString);
		NAN_METHOD(New);
		NAN_GETTER(GetX);
		NAN_GETTER(GetY);
		NAN_SETTER(SetX);
		NAN_SETTER(SetY);
		NAN_METHOD(ToString);
		
		SDL_Point* point_;
	};

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Color Wrapper/Unwrapper.
	// NAN_METHOD(ConstructColor);
	NAN_METHOD(ConstructColor);
	v8::Handle<v8::Object> WrapColor(SDL_Color* color);
	SDL_Color* UnwrapColor(v8::Handle<v8::Object> obj);

	// Property getters.
	NAN_GETTER(GetColorRed);
	NAN_GETTER(GetColorGreen);
	NAN_GETTER(GetColorBlue);
	NAN_GETTER(GetColorAlpha);
	// Property setters.
	NAN_SETTER(SetColorRed);
	NAN_SETTER(SetColorGreen);
	NAN_SETTER(SetColorBlue);
	NAN_SETTER(SetColorAlpha);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Palette Wrapper/Unwrapper.
	NAN_METHOD(ConstructPalette);
	v8::Handle<v8::Object> WrapPalette(SDL_Palette* palette);
	SDL_Palette* UnwrapPalette(v8::Handle<v8::Object> obj);
	// Property functions.
	NAN_GETTER(GetNcolors);
	NAN_GETTER(GetColors);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_DisplayMode Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapDisplayMode(SDL_DisplayMode* mode);
	SDL_DisplayMode* UnwrapDisplayMode(v8::Handle<v8::Value> val);
	NAN_GETTER(GetDisplayModeFormat);
	NAN_GETTER(GetDisplayModeWidth);
	NAN_GETTER(GetDisplayModeHeight);
	NAN_GETTER(GetDisplayModeRefreshRate);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_PixelFormat Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapPixelFormat(SDL_PixelFormat* pixelformat);
	SDL_PixelFormat* UnwrapPixelFormat(v8::Handle<v8::Object> obj);
	// Property getters.
	NAN_GETTER(GetFormatFormat);
	NAN_GETTER(GetFormatPalette);
	NAN_GETTER(GetFormatBits);
	NAN_GETTER(GetFormatBytes);
	NAN_GETTER(GetFormatRmask);
	NAN_GETTER(GetFormatGmask);
	NAN_GETTER(GetFormatBmask);
	NAN_GETTER(GetFormatAmask);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_RendererInfo Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapRendererInfo(SDL_RendererInfo* info);
	SDL_RendererInfo* UnwrapRendererInfo(v8::Handle<v8::Object> obj);
	// Property getters.
	NAN_GETTER(GetRendererInfoName);
	NAN_GETTER(GetRendererInfoFlags);
	NAN_GETTER(GetRendererInfoNumTextureFormats);
	NAN_GETTER(GetRendererInfoTextureFormats);
	NAN_GETTER(GetRendererInfoMaxTextureWidth);
	NAN_GETTER(GetRendererInfoMaxTextureHeight);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Joystick Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapJoystick(SDL_Joystick* joystick);
	SDL_Joystick* UnwrapJoystick(v8::Handle<v8::Object> obj);

	///////////////////////////////////////////////////////////////////////////////
	// TTF_Font Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapFont(TTF_Font* font);
	TTF_Font* UnwrapFont(v8::Handle<v8::Object> obj);
}

#endif
