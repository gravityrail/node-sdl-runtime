#include "mouse.h"
#include "surface.h"
#include "helpers.h"
#include "window.h"

using namespace v8;
using namespace node;


void sdl::mouse::Init(Handle<Object> exports) {
	CursorWrapper::Init(exports);

	NODE_SET_METHOD(exports, "showCursor", ShowCursor);
	NODE_SET_METHOD(exports, "getCursor", GetCursor);
	NODE_SET_METHOD(exports, "getDefaultCursor", GetDefaultCursor);
	NODE_SET_METHOD(exports, "getMouseFocus", GetMouseFocus);
	NODE_SET_METHOD(exports, "getMouseState", GetMouseState);
	NODE_SET_METHOD(exports, "getRelativeMouseMode", GetRelativeMouseMode);
	NODE_SET_METHOD(exports, "getRelativeMouseState", GetRelativeMouseState);

	NODE_SET_METHOD(exports, "setRelativeMouseMode", SetRelativeMouseMode);

	NODE_SET_METHOD(exports, "warpMouseInWindow", WarpMouseInWindow);

	NODE_SET_METHOD(exports, "button", ButtonMacroWrapper);

	Handle<Object> SYSTEM_CURSOR = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("SYSTEM_CURSOR"), SYSTEM_CURSOR);
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("ARROW"), Nan::New<Number>(SDL_SYSTEM_CURSOR_ARROW));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("IBEAM"), Nan::New<Number>(SDL_SYSTEM_CURSOR_IBEAM));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("WAIT"), Nan::New<Number>(SDL_SYSTEM_CURSOR_WAIT));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("CROSSHAIR"), Nan::New<Number>(SDL_SYSTEM_CURSOR_CROSSHAIR));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("WAITARROW"), Nan::New<Number>(SDL_SYSTEM_CURSOR_WAITARROW));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("SIZENWSE"), Nan::New<Number>(SDL_SYSTEM_CURSOR_SIZENWSE));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("SIZENESW"), Nan::New<Number>(SDL_SYSTEM_CURSOR_SIZENESW));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("SIZEWE"), Nan::New<Number>(SDL_SYSTEM_CURSOR_SIZEWE));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("SIZENS"), Nan::New<Number>(SDL_SYSTEM_CURSOR_SIZENS));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("SIZEALL"), Nan::New<Number>(SDL_SYSTEM_CURSOR_SIZEALL));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("NO"), Nan::New<Number>(SDL_SYSTEM_CURSOR_NO));
	Nan::Set(SYSTEM_CURSOR, Nan::New<String>("HAND"), Nan::New<Number>(SDL_SYSTEM_CURSOR_HAND));

	Handle<Object> BUTTON = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("BUTTON"), BUTTON);
	Nan::Set(BUTTON, Nan::New<String>("LEFT"), Nan::New<Number>(SDL_BUTTON_LEFT));
	Nan::Set(BUTTON, Nan::New<String>("MIDDLE"), Nan::New<Number>(SDL_BUTTON_MIDDLE));
	Nan::Set(BUTTON, Nan::New<String>("RIGHT"), Nan::New<Number>(SDL_BUTTON_RIGHT));
	Nan::Set(BUTTON, Nan::New<String>("X1"), Nan::New<Number>(SDL_BUTTON_X1));
	Nan::Set(BUTTON, Nan::New<String>("X2"), Nan::New<Number>(SDL_BUTTON_X2));
	Nan::Set(BUTTON, Nan::New<String>("LMASK"), Nan::New<Number>(SDL_BUTTON_LMASK));
	Nan::Set(BUTTON, Nan::New<String>("MMASK"), Nan::New<Number>(SDL_BUTTON_MMASK));
	Nan::Set(BUTTON, Nan::New<String>("RMASK"), Nan::New<Number>(SDL_BUTTON_RMASK));
	Nan::Set(BUTTON, Nan::New<String>("X1MASK"), Nan::New<Number>(SDL_BUTTON_X1MASK));
	Nan::Set(BUTTON, Nan::New<String>("X2MASK"), Nan::New<Number>(SDL_BUTTON_X2MASK));
}

Persistent<FunctionTemplate> sdl::CursorWrapper::wrap_template_;
Persistent<FunctionTemplate> sdl::CursorWrapper::wrap_template_system_;

sdl::CursorWrapper::CursorWrapper() {
}
sdl::CursorWrapper::CursorWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}
sdl::CursorWrapper::~CursorWrapper() {
	if(NULL != cursor_) {
		SDL_FreeCursor(cursor_);
	}
}

void sdl::CursorWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("CursorWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "free", FreeCursor);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "set", SetCursor);

	Nan::Set(exports, Nan::New<String>("Cursor"), wrap_template_->GetFunction());

	// System cursor wrapper.
	tpl = FunctionTemplate::New(NewSystem);
	wrap_template_system_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_system_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_system_->SetClassName(Nan::New<String>("SystemCursorWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_system_, "free", FreeCursor);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_system_, "set", SetCursor);

	Nan::Set(exports, Nan::New<String>("SystemCursor"), wrap_template_system_->GetFunction());
}

NAN_METHOD(sdl::CursorWrapper::New) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("A new Cursor must be created with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected CreateColorCursor(Surface, Number, Number)")));
	}

	SurfaceWrapper* surface = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	int x = args[1]->Int32Value();
	int y = args[2]->Int32Value();
	SDL_Cursor* cursor = SDL_CreateColorCursor(surface->surface_, x, y);
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	CursorWrapper* wrap = new CursorWrapper();
	wrap->cursor_ = cursor;
	wrap->Wrap(args.This());

	return args.This();
}
NAN_METHOD(sdl::CursorWrapper::NewSystem) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("A new Cursor must be created with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected CreateColorCursor(Number)")));
	}

	SDL_SystemCursor id = static_cast<SDL_SystemCursor>(args[0]->Int32Value());
	SDL_Cursor* cursor = SDL_CreateSystemCursor(id);
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	CursorWrapper* wrap = new CursorWrapper();
	wrap->cursor_ = cursor;
	wrap->Wrap(args.This());

	return args.This();
}

// TODO: Implement this function. See:
//       http://wiki.libsdl.org/SDL_CreateCursor?highlight=%28\bCategoryMouse\b%29|%28CategoryEnum%29|%28CategoryStruct%29
// NAN_METHOD(sdl::CreateCursor) {
// 	HandleScope scope;

// 	return Undefined();
// }
NAN_METHOD(sdl::CursorWrapper::FreeCursor) {
	HandleScope scope;

	CursorWrapper* wrap = ObjectWrap::Unwrap<CursorWrapper>(args.This());
	SDL_FreeCursor(wrap->cursor_);
	wrap->cursor_ = NULL;

	return Undefined();
}
NAN_METHOD(sdl::CursorWrapper::SetCursor) {
	HandleScope scope;

	CursorWrapper* wrap = ObjectWrap::Unwrap<CursorWrapper>(args.This());
	SDL_SetCursor(wrap->cursor_);

	return Undefined();
}

NAN_METHOD(sdl::ShowCursor) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected Cursor.show(Number)")));
	}

	int toggle = args[0]->Int32Value();
	int err = SDL_ShowCursor(toggle);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(err));
}
NAN_METHOD(sdl::GetCursor) {
	HandleScope scope;

	SDL_Cursor* cursor = SDL_GetCursor();
	if(NULL == cursor) {
		return Null();
	}

	Handle<Object> toWrap = Nan::New<Object>();
	CursorWrapper* wrap = new CursorWrapper(toWrap);
	wrap->cursor_ = cursor;

	return scope.Close(toWrap);
}
NAN_METHOD(sdl::GetDefaultCursor) {
	HandleScope scope;

	SDL_Cursor* cursor = SDL_GetDefaultCursor();
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> toWrap = Nan::New<Object>();
	CursorWrapper* wrap = new CursorWrapper(toWrap);
	wrap->cursor_ = cursor;

	return scope.Close(toWrap);

	return Undefined();
}
NAN_METHOD(sdl::GetMouseFocus) {
	HandleScope scope;

	SDL_Window* window = SDL_GetMouseFocus();
	if(NULL == window) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> toWrap = Nan::New<Object>();
	WindowWrapper* wrap = new WindowWrapper(toWrap);
	wrap->window_ = window;

	return scope.Close(toWrap);
}
NAN_METHOD(sdl::GetMouseState) {
	HandleScope scope;

	int x, y;
	uint32_t mask = SDL_GetMouseState(&x, &y);

	Handle<Array> ret = Array::New(3);
	Nan::Set(ret, 0, Nan::New<Number>(x));
	Nan::Set(ret, 1, Nan::New<Number>(y));
	Nan::Set(ret, 2, Nan::New<Number>(mask));

	return scope.Close(ret);
}
NAN_METHOD(sdl::GetRelativeMouseMode) {
	HandleScope scope;

	SDL_bool ret = SDL_GetRelativeMouseMode();

	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}
NAN_METHOD(sdl::GetRelativeMouseState) {
	HandleScope scope;

	int x, y;
	uint32_t mask = SDL_GetRelativeMouseState(&x, &y);

	Handle<Array> ret = Array::New(3);
	Nan::Set(ret, 0, Nan::New<Number>(x));
	Nan::Set(ret, 1, Nan::New<Number>(y));
	Nan::Set(ret, 2, Nan::New<Number>(mask));

	return scope.Close(ret);
}

NAN_METHOD(sdl::SetRelativeMouseMode) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected SetRelativeMouseMode(Boolean)")));
	}

	bool enabled = args[0]->BooleanValue();
	int err = SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::WarpMouseInWindow) {
	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected WarpMouseInWindow(Window, Number, Number)")));
	}

	WindowWrapper* window = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	int x = args[1]->Int32Value();
	int y = args[2]->Int32Value();
	SDL_WarpMouseInWindow(window->window_, x, y);

	return Undefined();
}

NAN_METHOD(sdl::ButtonMacroWrapper) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected button(Number)")));
	}

	int button = args[0]->Int32Value();
	int ret = SDL_BUTTON(button);

	return scope.Close(Nan::New<Number>(ret));
}
