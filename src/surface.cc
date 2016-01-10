#include "surface.h"
#include "helpers.h"
#include "struct_wrappers.h"
#include "container.h"
#include <iostream>

using namespace v8;
using namespace node;

Persistent<FunctionTemplate> sdl::SurfaceWrapper::wrap_template_;

sdl::SurfaceWrapper::SurfaceWrapper() {
}

sdl::SurfaceWrapper::SurfaceWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}

sdl::SurfaceWrapper::~SurfaceWrapper() {
	if(NULL != surface_) {
		SDL_FreeSurface(surface_);
	}
}

void sdl::SurfaceWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("SurfaceWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "loadBMP", LoadBMP);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "loadBMPRW", LoadBMPRW);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "saveBMP", SaveBMP);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "saveBMPRW", SaveBMPRW);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "blitScaled", BlitScaled);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "blitSurface", BlitSurface);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lowerBlit", LowerBlit);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lowerBlitScaled", LowerBlitScaled);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "convertSurface", ConvertSurface);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "convertSurfaceFormat", ConvertSurfaceFormat);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getWidth", GetWidth);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getHeight", GetHeight);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getPitch", GetPitch);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getPixelFormat", GetPixelFormat);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "fillRect", FillRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "fillRects", FillRects);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getClipRect", GetClipRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getColorKey", GetColorKey);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAlphaMod", GetAlphaMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getBlendMode", GetBlendMode);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getColorMod", GetColorMod);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setClipRect", SetClipRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setColorKey", SetColorKey);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setAlphaMod", SetAlphaMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setBlendMode", SetBlendMode);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setColorMod", SetColorMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setPalette", SetPalette);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setRLE", SetRLE);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lock", Lock);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "unlock", Unlock);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "mustLock", MustLock);

	Nan::Set(exports, Nan::New<String>("Surface"), wrap_template_->GetFunction());
}
NAN_METHOD(sdl::SurfaceWrapper::New) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Use the new operator to create instances of a Surface.")));
	}

	HandleScope scope;
	if(args[0]->IsExternal()) {
		SurfaceWrapper* obj = new SurfaceWrapper();
		obj->surface_ = static_cast<SDL_Surface*>(Handle<External>::Cast(args[0])->Value());
		obj->Wrap(args.This());
		return args.This();
	}
	else {
		if(args.Length() < 2) {
			return ThrowException(Exception::TypeError(
				Nan::New<String>("Invalid arguments: expected new sdl.Surface(Number, Number)")));
		}

		int flags = 0;
		int width = args[0]->Int32Value();
		int height = args[1]->Int32Value();
		int depth = args[2]->IsUndefined() ? 32 : args[2]->Int32Value();
		int rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    rmask = 0xff000000;
	    gmask = 0x00ff0000;
	    bmask = 0x0000ff00;
	    amask = 0x000000ff;
	#else
	    rmask = 0x000000ff;
	    gmask = 0x0000ff00;
	    bmask = 0x00ff0000;
	    amask = 0xff000000;
	#endif
	    SDL_Surface* surface = SDL_CreateRGBSurface(flags, width, height, depth, rmask, gmask, bmask, amask);
	    if(NULL == surface) {
	    	return ThrowSDLException(__func__);
	    }

	    SurfaceWrapper* obj = new SurfaceWrapper();
	    obj->surface_ = surface;
	    obj->Wrap(args.This());
	    return args.This();
	}
}

NAN_METHOD(sdl::SurfaceWrapper::LoadBMP) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected loadBMP(String)")));
	}

	String::Utf8Value file(args[0]);
	SDL_Surface* surface = SDL_LoadBMP(*file);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = surface;
	Handle<Object> ret = Nan::New<Object>();
	obj->Wrap(ret);

	return scope.Close(ret);
}
NAN_METHOD(sdl::SurfaceWrapper::LoadBMPRW) {
	// TODO: Implement LoadBMPRW.
	return ThrowException(Exception::TypeError(
		Nan::New<String>("LoadBMPRW is not implemented yet.")));
}
NAN_METHOD(sdl::SurfaceWrapper::SaveBMP) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected saveBMP(Surface, String)")));
	}

	SurfaceWrapper* wrap = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	String::Utf8Value file(args[1]);
	int err = SDL_SaveBMP(wrap->surface_, *file);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SaveBMPRW) {
	// TODO Implement SaveBMPRW.
	return ThrowException(Exception::TypeError(
		Nan::New<String>("SaveBMPRW is not implemented yet.")));
}

NAN_METHOD(sdl::SurfaceWrapper::BlitScaled) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected blitScaled(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_BlitScaled(obj->surface_,
		src == NULL ? NULL : src->wrapped,
		other->surface_,
		dst == NULL ? NULL : dst->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::BlitSurface) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected blitSurface(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_BlitSurface(obj->surface_, src->wrapped, other->surface_, dst == NULL ? NULL : dst->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::LowerBlit) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected lowerBlit(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_LowerBlit(obj->surface_, src->wrapped, other->surface_, dst == NULL ? NULL : dst->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::LowerBlitScaled) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected lowerBlitScaled(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_LowerBlitScaled(obj->surface_,
		src == NULL ? NULL : src->wrapped,
		other->surface_,
		dst == NULL ? NULL : dst->wrapped);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::ConvertSurface) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected convertSurface(PixelFormat)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_PixelFormat* fmt = UnwrapPixelFormat(Handle<Object>::Cast(args[0]));
	SDL_Surface* ret = SDL_ConvertSurface(self->surface_, fmt, 0);
	if(NULL == ret) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = ret;
	Handle<Object> objRet = Nan::New<Object>();
	obj->Wrap(objRet);
	return scope.Close(objRet);
}
NAN_METHOD(sdl::SurfaceWrapper::ConvertSurfaceFormat) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected convertSurfaceFormat(Number)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int fmt = args[0]->Int32Value();
	SDL_Surface* ret = SDL_ConvertSurfaceFormat(self->surface_, fmt, 0);
	if(NULL == ret) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = ret;
	Handle<Object> objRet = Nan::New<Object>();
	obj->Wrap(objRet);
	return scope.Close(objRet);
}

NAN_METHOD(sdl::SurfaceWrapper::FillRect) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected fillRect(Number[, Rect])")));
	}

	Handle<Object> handleObj = Handle<Object>::Cast(args.This());
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(handleObj);
	int color = args[0]->Int32Value();
	RectWrapper* rect = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	int err = SDL_FillRect(self->surface_, rect == NULL ? NULL : rect->wrapped, color);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::FillRects) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected fillRect(Number, Array)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int color = args[0]->Int32Value();
	Handle<Array> arr = Handle<Array>::Cast(args[1]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		RectWrapper* rect = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect->wrapped;
	}
	int err = SDL_FillRects(self->surface_, rects, numRects, color);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::SurfaceWrapper::GetClipRect) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_Rect* rect = new SDL_Rect;
	SDL_GetClipRect(self->surface_, rect);

  NEW_WRAPPED(rect, RectWrapper, ret)

	return scope.Close(ret);
}
NAN_METHOD(sdl::SurfaceWrapper::GetColorKey) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint32_t colorKey;
	int err = SDL_GetColorKey(self->surface_, &colorKey);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(colorKey));
}
NAN_METHOD(sdl::SurfaceWrapper::GetAlphaMod) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t alphaMod;
	int err = SDL_GetSurfaceAlphaMod(self->surface_, &alphaMod);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(alphaMod));
}
NAN_METHOD(sdl::SurfaceWrapper::GetBlendMode) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_BlendMode mode;
	int err = SDL_GetSurfaceBlendMode(self->surface_, &mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(mode));
}
NAN_METHOD(sdl::SurfaceWrapper::GetColorMod) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t r, g, b;
	int err = SDL_GetSurfaceColorMod(self->surface_, &r, &g, &b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Array> ret = Array::New(3);
	Nan::Set(ret, 0, Nan::New<Number>(r));
	Nan::Set(ret, 1, Nan::New<Number>(g));
	Nan::Set(ret, 2, Nan::New<Number>(b));
	return scope.Close(ret);
}
NAN_METHOD(sdl::SurfaceWrapper::GetWidth) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Nan::New<Number>(self->surface_->w));
}
NAN_METHOD(sdl::SurfaceWrapper::GetHeight) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Nan::New<Number>(self->surface_->h));
}
NAN_METHOD(sdl::SurfaceWrapper::GetPitch) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Nan::New<Number>(self->surface_->pitch));
}
NAN_METHOD(sdl::SurfaceWrapper::GetPixelFormat) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(WrapPixelFormat(self->surface_->format));
}

NAN_METHOD(sdl::SurfaceWrapper::SetClipRect) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setClipRect(Rect)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	RectWrapper* clip = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[0]));
	SDL_bool ret = SDL_SetClipRect(self->surface_, clip->wrapped);

	return scope.Close(Nan::New<Boolean>(ret));
}
NAN_METHOD(sdl::SurfaceWrapper::SetColorKey) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setColorKey(Boolean, Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int flag = args[0]->BooleanValue() ? 1 : 0;
	uint32_t key = args[1]->Int32Value();
	int err = SDL_SetColorKey(self->surface_, flag, key);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SetAlphaMod) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setAlphaMod(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t alpha = static_cast<uint8_t>(args[0]->Int32Value());
	int err = SDL_SetSurfaceAlphaMod(self->surface_, alpha);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SetBlendMode) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setBlendMode(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_BlendMode mode = static_cast<SDL_BlendMode>(args[0]->Int32Value());
	int err = SDL_SetSurfaceBlendMode(self->surface_, mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SetColorMod) {
	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setColorMod(Number, Number, Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t r = static_cast<uint8_t>(args[0]->Int32Value());
	uint8_t g = static_cast<uint8_t>(args[1]->Int32Value());
	uint8_t b = static_cast<uint8_t>(args[2]->Int32Value());
	int err = SDL_SetSurfaceColorMod(self->surface_, r, g, b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SetPalette) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setPalette(Palette)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_Palette* palette = UnwrapPalette(Handle<Object>::Cast(args[0]));
	int err = SDL_SetSurfacePalette(self->surface_, palette);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::SetRLE) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expected setRLE(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int flags = args[0]->Int32Value();
	int err = SDL_SetSurfaceRLE(self->surface_, flags);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::SurfaceWrapper::Lock) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int err = SDL_LockSurface(self->surface_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::Unlock) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_UnlockSurface(self->surface_);

	return Undefined();
}
NAN_METHOD(sdl::SurfaceWrapper::MustLock) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Nan::New<Boolean>(SDL_MUSTLOCK(self->surface_)));
}
