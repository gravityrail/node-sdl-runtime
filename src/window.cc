#include <string>
#include <iostream>

#include "window.h"
#include "helpers.h"
#include "struct_wrappers.h"
#include "surface.h"
#include "container.h"

using namespace v8;

////////////////////////////////////////////////////////////////////////////////
// WindowWrapper class definition.
Nan::Persistent<FunctionTemplate> sdl::WindowWrapper::window_wrap_template_;

sdl::WindowWrapper::WindowWrapper() {
}

sdl::WindowWrapper::WindowWrapper(Handle<Object> obj) {
	Wrap(obj);
}

sdl::WindowWrapper::~WindowWrapper() {
	std::cout << "WindowWrapper destructor running." << std::endl;
	if(NULL != window_) {
		std::cout << "Destroying contained window." << std::endl;
		SDL_DestroyWindow(window_);
	}
}

void sdl::WindowWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	window_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	window_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	window_wrap_template_->SetClassName(Nan::New<String>("WindowWrapper"));

	Nan::SetPrototypeMethod(window_wrap_template_, "getBrightness", GetBrightness);
	Nan::SetPrototypeMethod(window_wrap_template_, "getDisplayMode", GetDisplayMode);
	Nan::SetPrototypeMethod(window_wrap_template_, "getDisplayIndex", GetDisplayIndex);
	Nan::SetPrototypeMethod(window_wrap_template_, "getFlags", GetFlags);
	Nan::SetPrototypeMethod(window_wrap_template_, "getGammaRamp", GetGammaRamp);
	Nan::SetPrototypeMethod(window_wrap_template_, "getGrab", GetGrab);
	Nan::SetPrototypeMethod(window_wrap_template_, "getWindowID", GetWindowID);
	Nan::SetPrototypeMethod(window_wrap_template_, "getMaximumSize", GetMaximumSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "getMinimumSize", GetMinimumSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "getPixelFormat", GetPixelFormat);
	Nan::SetPrototypeMethod(window_wrap_template_, "getPosition", GetPosition);
	Nan::SetPrototypeMethod(window_wrap_template_, "getSize", GetSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "getSurface", GetSurface);
	Nan::SetPrototypeMethod(window_wrap_template_, "getTitle", GetTitle);

	Nan::SetPrototypeMethod(window_wrap_template_, "hide", Hide);
	Nan::SetPrototypeMethod(window_wrap_template_, "show", Show);

	Nan::SetPrototypeMethod(window_wrap_template_, "minimize", Minimize);
	Nan::SetPrototypeMethod(window_wrap_template_, "maximize", Maximize);

	Nan::SetPrototypeMethod(window_wrap_template_, "raise", Raise);
	Nan::SetPrototypeMethod(window_wrap_template_, "restore", Restore);

	Nan::SetPrototypeMethod(window_wrap_template_, "setBordered", SetBordered);
	Nan::SetPrototypeMethod(window_wrap_template_, "setBrightness", SetBrightness);
	Nan::SetPrototypeMethod(window_wrap_template_, "setDisplayMode", SetDisplayMode);
	Nan::SetPrototypeMethod(window_wrap_template_, "setFullscreen", SetFullscreen);
	Nan::SetPrototypeMethod(window_wrap_template_, "setGammeRamp", SetGammaRamp);
	Nan::SetPrototypeMethod(window_wrap_template_, "setGrab", SetGrab);
	Nan::SetPrototypeMethod(window_wrap_template_, "setIcon", SetIcon);
	Nan::SetPrototypeMethod(window_wrap_template_, "setMaximumSize", SetMaximumSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "setMinimumSize", SetMinimumSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "setPosition", SetPosition);
	Nan::SetPrototypeMethod(window_wrap_template_, "setSize", SetSize);
	Nan::SetPrototypeMethod(window_wrap_template_, "setTitle", SetTitle);

	Nan::SetPrototypeMethod(window_wrap_template_, "updateWindowSurface", UpdateWindowSurface);
	Nan::SetPrototypeMethod(window_wrap_template_, "updateWindowSurfaceRects", UpdateWindowSurfaceRects);

	Nan::Set(exports, Nan::New<String>("Window"), window_wrap_template_->GetFunction());
}

NAN_METHOD(sdl::WindowWrapper::New) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Use the new operator to create instances of a Window.")));
	}

	HandleScope scope;

	std::string title = info[0]->IsUndefined() ? "" : *(String::Utf8Value(info[0]));
	int x = info[1]->IsUndefined() ? SDL_WINDOWPOS_UNDEFINED : info[1]->Int32Value();
	int y = info[2]->IsUndefined() ? SDL_WINDOWPOS_UNDEFINED : info[2]->Int32Value();
	int w = info[3]->IsUndefined() ? 640 : info[3]->Int32Value();
	int h = info[4]->IsUndefined() ? 480 : info[4]->Int32Value();
	uint32_t flags = info[5]->IsUndefined() ? 0 : info[5]->Int32Value();

	WindowWrapper* obj = new WindowWrapper();
	obj->window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
	if(NULL == obj->window_) {
		delete obj;
		return ThrowSDLException("Window->New");
	}

	obj->Wrap(info.This());
	return scope.Close(info.This());
}

NAN_METHOD(sdl::WindowWrapper::GetBrightness) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
  // SDL documentation does not say this function can return an erroneous value, so
  // we don't do any error checking.
	return scope.Close(Nan::New<Number>(SDL_GetWindowBrightness(obj->window_)));
}

NAN_METHOD(sdl::WindowWrapper::GetDisplayIndex) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int ret = SDL_GetWindowDisplayIndex(obj->window_);
  // SDL documentation says that values less than 0 are returned upon error.
	if(ret < 0) {
		return ThrowSDLException("Window->GetDisplayIndex");
	}

	return scope.Close(Nan::New<Number>(ret));
}

NAN_METHOD(sdl::WindowWrapper::GetDisplayMode) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_DisplayMode mode;
	int err = SDL_GetWindowDisplayIndex(obj->window_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	err = SDL_GetCurrentDisplayMode(err, &mode);
  // SDL documentation says that 0 is success, and less than 0 is when an error
  // occurred.
	if(err < 0) {
		return ThrowSDLException("Window->GetDisplayMode");
	}

	return scope.Close(SDLDisplayModeToJavascriptObject(mode));
}

NAN_METHOD(sdl::WindowWrapper::GetFlags) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
  // TODO: Return an array of human-readable strings denoting each flag instead?
  // SDL documentation does not say this function can return an error code.
	return scope.Close(Nan::New<Number>(SDL_GetWindowFlags(obj->window_)));
}

NAN_METHOD(sdl::WindowWrapper::GetGammaRamp) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	uint16_t redArr[256];
	uint16_t greenArr[256];
	uint16_t blueArr[256];
	int err = SDL_GetWindowGammaRamp(obj->window_, redArr, greenArr, blueArr);
	if(err < 0) {
		return ThrowSDLException("Window->GetGammaRamp");
	}

	Handle<Array> ret = Array::New(3);
	Handle<Array> jsRedArr = Array::New(256);
	Handle<Array> jsGreenArr = Array::New(256);
	Handle<Array> jsBlueArr = Array::New(256);
	for(int i = 0; i < 256; i++) {
		Nan::Set(jsRedArr, i, Nan::New<Number>(redArr[i]));
		Nan::Set(jsGreenArr, i, Nan::New<Number>(greenArr[i]));
		Nan::Set(jsBlueArr, i, Nan::New<Number>(blueArr[i]));
	}

	Nan::Set(ret, 0, jsRedArr);
	Nan::Set(ret, 1, jsGreenArr);
	Nan::Set(ret, 2, jsBlueArr);
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetGrab) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	return scope.Close(Nan::New<Boolean>(SDL_GetWindowGrab(obj->window_)));
}

NAN_METHOD(sdl::WindowWrapper::GetWindowID) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	return scope.Close(Nan::New<Number>(SDL_GetWindowID(obj->window_)));
}

NAN_METHOD(sdl::WindowWrapper::GetMaximumSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int w, h;
	SDL_GetWindowMaximumSize(obj->window_, &w, &h);
	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(w));
	Nan::Set(ret, 1, Nan::New<Number>(h));
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetMinimumSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int w, h;
	SDL_GetWindowMinimumSize(obj->window_, &w, &h);
	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(w));
	Nan::Set(ret, 1, Nan::New<Number>(h));
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetPixelFormat) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	uint32_t ret = SDL_GetWindowPixelFormat(obj->window_);
	if(SDL_PIXELFORMAT_UNKNOWN == ret) {
		return ThrowSDLException("Window->GetPixelFormat");
	}

	return scope.Close(Nan::New<Number>(ret));
}

NAN_METHOD(sdl::WindowWrapper::GetPosition) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int x, y;
	SDL_GetWindowPosition(obj->window_, &x, &y);

	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 1, Nan::New<Number>(x));
	Nan::Set(ret, 2, Nan::New<Number>(y));
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int w, h;
	SDL_GetWindowSize(obj->window_, &w, &h);

	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 1, Nan::New<Number>(w));
	Nan::Set(ret, 2, Nan::New<Number>(h));
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetSurface) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_Surface* surf = SDL_GetWindowSurface(obj->window_);
	if(NULL == surf) {
		return ThrowSDLException("Window->GetSurface");
	}

	Handle<Object> ret = Nan::New<Object>();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surf;
	return scope.Close(ret);
}

NAN_METHOD(sdl::WindowWrapper::GetTitle) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	return scope.Close(Nan::New<String>(SDL_GetWindowTitle(obj->window_)));
}

NAN_METHOD(sdl::WindowWrapper::Hide) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_HideWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Show) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_ShowWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Maximize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_MaximizeWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Minimize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_MinimizeWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Raise) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_RaiseWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Restore) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_RestoreWindow(obj->window_);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetBordered) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	bool bordered = info[0]->IsUndefined() ? true : info[0]->BooleanValue();
	SDL_SetWindowBordered(obj->window_, bordered ? SDL_TRUE : SDL_FALSE);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetBrightness) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	double brightness = info[0]->IsUndefined() ? 1.0 : info[0]->NumberValue();
	if(brightness > 1.0) {
		brightness = 1.0;
	}
	else if(brightness < 0.0) {
		brightness = 0.0;
	}

	SDL_SetWindowBrightness(obj->window_, static_cast<float>(brightness));
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetDisplayMode) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info[0]->IsUndefined()) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetDisplayMode(DisplayMode)")));
	}
	SDL_DisplayMode* mode = UnwrapDisplayMode(info[0]);
	int err = SDL_SetWindowDisplayMode(obj->window_, mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetFullscreen) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info[0]->IsUndefined()) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetFullscreen(Number)")));
	}
	int fullscreen = info[0]->Int32Value();
	int err = SDL_SetWindowFullscreen(obj->window_, fullscreen);
	if(err < 0) {
		return ThrowSDLException("Window->SetFullscreen");
	}
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetGammaRamp) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 3 ) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetGammaRamp(Array, Array, Array)")));
	}
	if(!info[0]->IsArray() || !info[1]->IsArray() || !info[2]->IsArray()) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetGammaRamp(Array, Array, Array)")));
	}

	Handle<Array> red = Handle<Array>::Cast(info[0]);
	Handle<Array> green = Handle<Array>::Cast(info[1]);
	Handle<Array> blue = Handle<Array>::Cast(info[2]);
	uint16_t redArr[256];
	uint16_t greenArr[256];
	uint16_t blueArr[256];
	for(int i = 0; i < 256; i++) {
		redArr[i] = red->CloneElementAt(i)->Int32Value();
		greenArr[i] = green->CloneElementAt(i)->Int32Value();
		blueArr[i] = blue->CloneElementAt(i)->Int32Value();
	}
	int err = SDL_SetWindowGammaRamp(obj->window_, redArr, greenArr, blueArr);
	if(err < 0) {
		return ThrowSDLException("Window->SetGammaRamp");
	}

	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetGrab) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetGrab(Boolean)")));
	}
	bool grab = info[0]->ToBoolean()->BooleanValue();
	SDL_SetWindowGrab(obj->window_, grab ? SDL_TRUE : SDL_FALSE);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetIcon) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetIcon(Surface)")));
	}
	SurfaceWrapper* wrap = Nan::ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(info[0]));
	SDL_SetWindowIcon(obj->window_, wrap->surface_);

	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetMaximumSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetMaximumSize(Number, Number)")));
	}
	int max_w = info[0]->Int32Value();
	int max_h = info[0]->Int32Value();
	SDL_SetWindowMaximumSize(obj->window_, max_w, max_h);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetMinimumSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetMinimumSize(Number, Number)")));
	}
	int min_w = info[0]->Int32Value();
	int min_h = info[0]->Int32Value();
	SDL_SetWindowMinimumSize(obj->window_, min_w, min_h);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetPosition) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetPosition(Number, Number)")));
	}
	int x = info[0]->Int32Value();
	int y = info[0]->Int32Value();
	SDL_SetWindowPosition(obj->window_, x, y);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetSize) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetSize(Number, Number)")));
	}
	int w = info[0]->Int32Value();
	int h = info[0]->Int32Value();
	SDL_SetWindowSize(obj->window_, w, h);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::SetTitle) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetTitle(String)")));
	}
	String::Utf8Value title(info[0]);
	SDL_SetWindowTitle(obj->window_, *title);
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::UpdateWindowSurface) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	int err = SDL_UpdateWindowSurface(obj->window_);
	if(err < 0) {
		return ThrowSDLException("Window->SetTitle");
	}
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::UpdateWindowSurfaceRects) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected UpdateWindowSurfaceRects(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(info[0]);
	int len = arr->Length();
	SDL_Rect* rects = new SDL_Rect[len];
	for(int i = 0; i < len; i++) {
		RectWrapper* wrap = Nan::ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *wrap->wrapped;
	}

	int err = SDL_UpdateWindowSurfaceRects(obj->window_, rects, len);
	delete rects;
	if(err < 0) {
		return ThrowSDLException("Window->SetTitle");
	}
	return Undefined();
}

NAN_METHOD(sdl::WindowWrapper::Swap) {
	HandleScope scope;

	WindowWrapper* obj = Nan::ObjectWrap::Unwrap<WindowWrapper>(info.This());
	SDL_GL_SwapWindow(obj->window_);
	return Undefined();
}
