#include "font.h"
#include "helpers.h"
#include "container.h"
#include "surface.h"

#include <iostream>

using namespace v8;
using namespace node;


Nan::Persistent<FunctionTemplate> sdl::TTF::FontWrapper::wrap_template_;

sdl::TTF::FontWrapper::FontWrapper() {
}
sdl::TTF::FontWrapper::~FontWrapper() {
	if(NULL != font_) {
		TTF_CloseFont(font_);
	}
}

void sdl::TTF::FontWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("FontWrapper"));

	Nan::SetPrototypeMethod(wrap_template_, "renderTextSolid", RenderTextSolid);
	Nan::SetPrototypeMethod(wrap_template_, "renderUtf8Solid", RenderUTF8Solid);
	Nan::SetPrototypeMethod(wrap_template_, "renderUnicodeSolid", RenderUnicodeSolid);
	Nan::SetPrototypeMethod(wrap_template_, "renderGlyphSolid", RenderGlyphSolid);

	Nan::SetPrototypeMethod(wrap_template_, "renderTextShaded", RenderTextShaded);
	Nan::SetPrototypeMethod(wrap_template_, "renderUtf8Shaded", RenderUTF8Shaded);
	Nan::SetPrototypeMethod(wrap_template_, "renderUnicodeShaded", RenderUnicodeShaded);
	Nan::SetPrototypeMethod(wrap_template_, "renderGlyphShaded", RenderGlyphShaded);

	Nan::SetPrototypeMethod(wrap_template_, "renderTextBlended", RenderTextBlended);
	Nan::SetPrototypeMethod(wrap_template_, "renderUtf8Blended", RenderUTF8Blended);
	Nan::SetPrototypeMethod(wrap_template_, "renderUnicodeBlended", RenderUnicodeBlended);
	Nan::SetPrototypeMethod(wrap_template_, "renderGlyphBlended", RenderGlyphBlended);

	Nan::Set(exports, Nan::New<String>("Font").ToLocalChecked(), wrap_template_->GetFunction()));
}
NAN_METHOD(sdl::TTF::FontWrapper::New) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("A Font must be created with the new operator.")));
	}

	HandleScope scope;

	if(info[0]->IsExternal()) {
		FontWrapper* obj = new FontWrapper();
		obj->font_ = static_cast<TTF_Font*>(Handle<External>::Cast(info[0])->Value());
		obj->Wrap(info.This());
		return info.This();
	}
	else {
		if(!info[0]->IsString()) {
			return ThrowException(Exception::TypeError(
				Nan::New<String>("Invalid arguments: First argument to new sdl.Font must be a String.")));
		}
		if(!info[1]->IsNumber()) {
			return ThrowException(Exception::TypeError(
				Nan::New<String>("Invalid arguments: Second argument to new sdl.Font must be a Number.")));
		}

		String::Utf8Value file(info[0]);
		int ptsize = info[1]->Int32Value();
		TTF_Font* font = TTF_OpenFont(*file, ptsize);
		if(NULL == font) {
			return ThrowSDLException(__func__);
		}

		FontWrapper* obj = new FontWrapper();
		obj->font_ = font;
		obj->Wrap(info.This());
		return info.This();
	}
}

NAN_METHOD(sdl::TTF::FontWrapper::RenderTextSolid) {
	HandleScope scope;
	Context::Scope context_scope(Context::GetCurrent());

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderTextSolid must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderTextSolid must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderTextSolid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderText_Solid(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUTF8Solid) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderUtf8Solid must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUtf8Solid must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUtf8Solid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Solid(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUnicodeSolid) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUnicodeSolid must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUnicodeSolid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Solid(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderGlyphSolid) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderGlyphSolid must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderGlyphSolid must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderGlyphSolid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Solid(font->font_, (*text)[0], *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}

NAN_METHOD(sdl::TTF::FontWrapper::RenderTextShaded) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderTextShaded must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderTextShaded must be an sdl.Color.")));
	}
	if(!info[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Third argument to renderTextShaded must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* fg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderTextShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap third argument to renderTextShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderText_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUTF8Shaded) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderUtf8Shaded must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUtf8Shaded must be an sdl.Color.")));
	}
	if(!info[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Third argument to renderUtf8Shaded must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* fg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUtf8Shaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap third argument to renderUtf8Shaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUnicodeShaded) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderUnicodeShaded must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUnicodeShaded must be an sdl.Color.")));
	}
	if(!info[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Third argument to renderUnicodeShaded must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	ColorWrapper* fg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap third argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderGlyphShaded) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderUnicodeShaded must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUnicodeShaded must be an sdl.Color.")));
	}
	if(!info[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Third argument to renderUnicodeShaded must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderGlyphShaded must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* fg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap third argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Shaded(font->font_, (*text)[0], *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}

NAN_METHOD(sdl::TTF::FontWrapper::RenderTextBlended) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderTextBlended must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderTextBlended must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderTextBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUTF8Blended) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderUtf8Blended must be a String.")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUtf8Blended must be an sdl.Color.")));
	}

	String::Utf8Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUtf8Blended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderUnicodeBlended) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderUnicodeBlended must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderUnicodeBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}
NAN_METHOD(sdl::TTF::FontWrapper::RenderGlyphBlended) {
	HandleScope scope;

	FontWrapper* font = Nan::ObjectWrap::Unwrap<FontWrapper>(info.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!info[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Second argument to renderGlyphBlended must be an sdl.Color.")));
	}

	String::Value text(info[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: First argument to renderGlyphBlended must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* color = Nan::ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(info[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Could not unwrap second argument to renderGlyphBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Blended(font->font_, (*text)[0], *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	return scope.Close(ret);
}

void sdl::TTF::Initialize(Handle<Object> exports) {
	Handle<Object> TTF = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("TTF").ToLocalChecked(), TTF));
	Nan::SetMethod(TTF, "init", Init);
	Nan::SetMethod(TTF, "wasInit", WasInit);
	Nan::SetMethod(TTF, "quit", Quit);
	Nan::SetMethod(TTF, "getError", GetError);

	FontWrapper::Init(TTF);
}

NAN_METHOD(sdl::TTF::Init) {
	HandleScope scope;

	if (!(info.Length() == 0)) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected TTF::Init()")));
	}

	if (TTF_Init() < 0) {
		return ThrowException(Exception::Error(String::Concat(
			Nan::New<String>("TTF::Init: "),
			Nan::New<String>(TTF_GetError())
			)));
	}

	return Undefined();
}

NAN_METHOD(sdl::TTF::WasInit) {
	HandleScope scope;

	return scope.Close(Nan::New<Boolean>(TTF_WasInit() ? true : false));
}

NAN_METHOD(sdl::TTF::Quit) {
	HandleScope scope;

	TTF_Quit();

	return Undefined();
}

// NAN_METHOD(sdl::TTF::SetError) {
// 	HandleScope scope;

// 	return Undefined();
// }
NAN_METHOD(sdl::TTF::GetError) {
	HandleScope scope;

	const char* error = TTF_GetError();
	// Can this happen?!
	if(NULL == error) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(error));
}

// NAN_METHOD(sdl::TTF::OpenFont) {
// 	HandleScope scope;

// 	if (!(info.Length() == 2 && info[0]->IsString() && info[1]->IsNumber())) {
// 		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected TTF::OpenFont(String, Number)")));
// 	}

// 	String::Utf8Value file(info[0]);
// 	int ptsize = (info[1]->Int32Value());

// 	TTF_Font* font = TTF_OpenFont(*file, ptsize);
// 	if (font == NULL) {
// 		return ThrowException(Exception::Error(String::Concat(
// 			Nan::New<String>("TTF::OpenFont: "),
// 			Nan::New<String>(TTF_GetError())
// 			)));
// 	}
// 	return Undefined();
// 	// return scope.Close(WrapFont(font));
// }

// TODO: Rewrite for SDL2.
// static NAN_METHOD(sdl::TTF::RenderTextBlended) {
//   HandleScope scope;

//   if (!(info.Length() == 3 && info[0]->IsObject() && info[1]->IsString() && info[2]->IsNumber())) {
//     return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected TTF::RenderTextBlended(Font, String, Number)")));
//   }

//   SDL_PixelFormat* vfmt = SDL_GetVideoInfo()->vfmt;
//   TTF_Font* font = UnwrapFont(info[0]->ToObject());
//   String::Utf8Value text(info[1]);
//   int colorCode = info[2]->Int32Value();

//   Uint8 r, g, b;
//   SDL_GetRGB(colorCode, vfmt, &r, &g, &b);

//   SDL_Color color;
//   color.r = r;
//   color.g = g;
//   color.b = b;

//   SDL_Surface *resulting_text;
//   resulting_text = TTF_RenderText_Blended(font, *text, color);
//   if (!resulting_text) {
//     return ThrowException(Exception::Error(String::Concat(
//       Nan::New<String>("TTF::RenderTextBlended: "),
//       Nan::New<String>(TTF_GetError())
//     )));
//   }
//   return scope.Close(WrapSurface(resulting_text));
// }
