#ifndef NODESDL_CONTAINER_H
#define NODESDL_CONTAINER_H

#include <node.h>
#include <v8.h>

#include "SDL.h"
#include "helpers.h"


namespace sdl {
	OPEN_OBJECTWRAP(RectWrapper)
		static GETTER_DEF(GetX);
		static GETTER_DEF(GetY);
		static GETTER_DEF(GetW);
		static GETTER_DEF(GetH);

		static SETTER_DEF(SetX);
		static SETTER_DEF(SetY);
		static SETTER_DEF(SetW);
		static SETTER_DEF(SetH);
	CLOSE_OBJECTWRAP(SDL_Rect)
	// class RectWrapper : public node::ObjectWrap {
	// public:
	// 	static v8::Persistent<v8::FunctionTemplate> wrap_template_;

	// 	RectWrapper();
	// 	RectWrapper(v8::Handle<v8::Object> toWrap);
	// 	~RectWrapper();

	// 	static void Init(v8::Handle<v8::Object> exports);
	// 	static NAN_METHOD(New);

	// 	static GETTER_DEF(GetX);
	// 	static GETTER_DEF(GetY);
	// 	static GETTER_DEF(GetW);
	// 	static GETTER_DEF(GetH);

	// 	static void SetX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	// 	static void SetY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	// 	static void SetW(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	// 	static void SetH(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	// 	SDL_Rect* wrapped;
	// };

	class ColorWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		~ColorWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		NAN_METHOD(New);

		NAN_GETTER(GetRed);
		NAN_GETTER(GetGreen);
		NAN_GETTER(GetBlue);
		NAN_GETTER(GetAlpha);
		NAN_GETTER(GetColor);

		NAN_SETTER(SetRed);
		NAN_SETTER(SetGreen);
		NAN_SETTER(SetBlue);
		NAN_SETTER(SetAlpha);

		NAN_METHOD(ToString);

		SDL_Color* color_;
	};

	class FingerWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		FingerWrapper();
		FingerWrapper(v8::Handle<v8::Object> toWrap);
		~FingerWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		NAN_METHOD(New);

		NAN_GETTER(GetFingerID);
		NAN_GETTER(GetX);
		NAN_GETTER(GetY);
		NAN_GETTER(GetPressure);

		SDL_Finger* finger_;
	};
}

#endif
