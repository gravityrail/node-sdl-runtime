#include <iostream>

#include "texture.h"
#include "surface.h"
#include "helpers.h"
#include "struct_wrappers.h"
#include "render.h"
#include "container.h"

#include "SDL.h"

using namespace v8;

////////////////////////////////////////////////////////////////////////////////
// TextureWrapper Class Definition.
Nan::Persistent<FunctionTemplate> sdl::TextureWrapper::texture_wrap_template_;

sdl::TextureWrapper::TextureWrapper() {
}

sdl::TextureWrapper::TextureWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}

sdl::TextureWrapper::~TextureWrapper() {
	if(NULL != texture_) {
		SDL_DestroyTexture(texture_);
	}
}

void sdl::TextureWrapper::Init(Handle<Object> exports) {
  // Setup hardware renderer construction.
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	texture_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	texture_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	texture_wrap_template_->SetClassName(Nan::New<String>("TextureWrapper"));

	Nan::SetPrototypeMethod(texture_wrap_template_, "getAlphaMod", GetAlphaMod);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getBlendMode", GetBlendMode);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getColorMod", GetColorMod);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getFormat", GetFormat);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getSize", GetSize);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getWidth", GetWidth);
	Nan::SetPrototypeMethod(texture_wrap_template_, "getHeight", GetHeight);

	Nan::SetPrototypeMethod(texture_wrap_template_, "setAlphaMod", SetAlphaMod);
	Nan::SetPrototypeMethod(texture_wrap_template_, "setBlendMode", SetBlendMode);
	Nan::SetPrototypeMethod(texture_wrap_template_, "setColorMod", SetColorMod);

	// Nan::SetPrototypeMethod(texture_wrap_template_, "lock", Lock);
	// Nan::SetPrototypeMethod(texture_wrap_template_, "unlock", Unlock);

	Nan::SetPrototypeMethod(texture_wrap_template_, "update", Update);

	Nan::Set(exports, Nan::New<String>("Texture"), texture_wrap_template_->GetFunction());
}

NAN_METHOD(sdl::TextureWrapper::New) {
	// std::cout << "Texture::New - Checking for constructor call." << std::endl;
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Use the new operator to create instances of a Texture.")));
	}

	HandleScope scope;

	SDL_Texture* tex;
	// std::cout << "Texture::New - Unwrapping RendererWrapper from first argument." << std::endl;
	RendererWrapper* r = Nan::ObjectWrap::Unwrap<RendererWrapper>(Handle<Object>::Cast(info[0]));
	// std::cout << "Texture::New - Done. Checking arguments length." << std::endl;
	if(info.Length() > 2) {
		// std::cout << "Texture::New - Found more than 2 arguments. Pulling format, access, w, h." << std::endl;
		int format = info[1]->IsUndefined() ? SDL_PIXELFORMAT_UNKNOWN : info[1]->Int32Value();
		int access = info[2]->IsUndefined() ? SDL_TEXTUREACCESS_STREAMING : info[2]->Int32Value();
		int w = info[3]->IsUndefined() ? 1 : info[3]->Int32Value();
		int h = info[4]->IsUndefined() ? 1 : info[4]->Int32Value();
		// std::cout << "Texture::New - Done. Calling SDL_CreateTexture." << std::endl;
		tex = SDL_CreateTexture(r->renderer_, format, access, w, h);
		// std::cout << "Texture::New - Done." << std::endl;
	}
	else if(info.Length() == 2) {
		// std::cout << "Texture::New - Found 2 arguments. Pulling SurfaceWrapper from second arg." << std::endl;
		SurfaceWrapper* wrap = Nan::ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(info[1]));
		// std::cout << "Texture::New - Done. Calling SDL_CreateTextureFromSurface." << std::endl;
		tex = SDL_CreateTextureFromSurface(r->renderer_, wrap->surface_);
		// std::cout << "Texture::New - Done." << std::endl;
	}
	else {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: expecting new sdl.Texture(Renderer, Surface) or new sdl.Texture(Renderer, Number, Number, Number, Number)")));
	}
	if(NULL == tex) {
		return ThrowSDLException(__func__);
	}

	// std::cout << "Texture::New - Creating new TextureWrapper." << std::endl;
	TextureWrapper* obj = new TextureWrapper();
	// std::cout << "Texture::New - Setting texture_ to created tex." << std::endl;
	obj->texture_ = tex;
	// std::cout << "Texture::New - Wrapping info.This()." << std::endl;
	obj->Wrap(info.This());

	// std::cout << "Texture::New - Returning info.This()." << std::endl;
	return info.This();
}

NAN_METHOD(sdl::TextureWrapper::GetAlphaMod) {
	HandleScope scope;

	uint8_t alpha;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_GetTextureAlphaMod(tex->texture_, &alpha);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(alpha));
}
NAN_METHOD(sdl::TextureWrapper::GetBlendMode) {
	HandleScope scope;

	SDL_BlendMode mode;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_GetTextureBlendMode(tex->texture_, &mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(mode));
}
NAN_METHOD(sdl::TextureWrapper::GetColorMod) {
	HandleScope scope;

	uint8_t r, g, b;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_GetTextureColorMod(tex->texture_, &r, &g, &b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Nan::New<Object>();
	Nan::Set(ret, Nan::New<String>("r"), Nan::New<Number>(r));
	Nan::Set(ret, Nan::New<String>("g"), Nan::New<Number>(g));
	Nan::Set(ret, Nan::New<String>("b"), Nan::New<Number>(b));

	return scope.Close(ret);
}
NAN_METHOD(sdl::TextureWrapper::GetFormat) {
	HandleScope scope;

	uint32_t format;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_QueryTexture(tex->texture_, &format, NULL, NULL, NULL);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(format));
}
NAN_METHOD(sdl::TextureWrapper::GetSize) {
	HandleScope scope;

	int access;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_QueryTexture(tex->texture_, NULL, &access, NULL, NULL);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(access));
}
NAN_METHOD(sdl::TextureWrapper::GetWidth) {
	HandleScope scope;

	int width;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_QueryTexture(tex->texture_, NULL, NULL, &width, NULL);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(width));
}
NAN_METHOD(sdl::TextureWrapper::GetHeight) {
	HandleScope scope;

	int height;
	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_QueryTexture(tex->texture_, NULL, NULL, NULL, &height);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(height));
}

NAN_METHOD(sdl::TextureWrapper::SetAlphaMod) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected texture.setAlphaMod(Number)")));
	}

	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_SetTextureAlphaMod(tex->texture_, static_cast<uint8_t>(info[0]->Uint32Value()));
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::TextureWrapper::SetBlendMode) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected texture.setBlendMode(Number)")));
	}

	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int err = SDL_SetTextureBlendMode(tex->texture_, static_cast<SDL_BlendMode>(info[0]->Int32Value()));
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::TextureWrapper::SetColorMod) {
	HandleScope scope;

	if(info.Length() < 3) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected texture.setColorMod(Number, Number, Number)")));
	}

	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
	int r = info[0]->Int32Value();
	int g = info[1]->Int32Value();
	int b = info[2]->Int32Value();
	int err = SDL_SetTextureColorMod(tex->texture_, r, g, b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

// NAN_METHOD(sdl::TextureWrapper::Lock) {
// 	HandleScope scope;

// 	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
// 	int err = SDL_LockTexture(tex->texture_);
// 	if(err < 0) {
// 		return ThrowSDLException(__func__);
// 	}

// 	return Undefined();
// }
// NAN_METHOD(sdl::TextureWrapper::Unlock) {
// 	HandleScope scope;

// 	TextureWrapper* tex = Nan::ObjectWrap::Unwrap<TextureWrapper>(info.This());
// 	int err = SDL_UnlockTexture(tex->texture_);
// 	if(err < 0) {
// 		return ThrowSDLException(__func__);
// 	}

// 	return Undefined();
// }

NAN_METHOD(sdl::TextureWrapper::Update) {
	HandleScope scope;

	if(!info[0]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to texture.update must be an Object.")));
	}

	TextureWrapper* texture = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info.This()));
	if(NULL == texture) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Failed to unwrap this argument to a SurfaceWrapper. (is this not an sdl.Texture?)")));
	}
	SurfaceWrapper* surface = Nan::ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(info[0]));
	if(NULL == surface) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Failed to unwrap first argument to a SurfaceWrapper. (did you not pass in an sdl.Surface?)")));
	}
	RectWrapper* rect = info[1]->IsUndefined() ? NULL : Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info[1]));
	int err = SDL_UpdateTexture(texture->texture_, rect == NULL ? NULL : rect->wrapped, surface->surface_->pixels, surface->surface_->pitch);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
