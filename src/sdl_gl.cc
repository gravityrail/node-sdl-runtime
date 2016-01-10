#include "sdl_gl.h"
#include "window.h"
#include "helpers.h"
#include "texture.h"

using namespace v8;
using namespace node;


void sdl::gl::Init(Handle<Object> exports) {
	Local<Object> GL = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("GL").ToLocalChecked(), GL));
	ContextWrapper::Init(GL);

	Nan::SetMethod(GL, "bindTexture", BindTexture);
	Nan::SetMethod(GL, "unbindTexture", UnbindTexture);

	Nan::SetMethod(GL, "extensionSupported", ExtensionSupported);

	Nan::SetMethod(GL, "loadLibrary", LoadLibrary);
	Nan::SetMethod(GL, "unloadLibrary", UnloadLibrary);

	Nan::SetMethod(GL, "setAttribute", SetAttribute);
	Nan::SetMethod(GL, "makeCurrent", MakeCurrent);
	Nan::SetMethod(GL, "setSwapInterval", SetSwapInterval);

	Nan::SetMethod(GL, "getAttribute", GetAttribute);
	Nan::SetMethod(GL, "getCurrentContext", GetCurrentContext);
	Nan::SetMethod(GL, "getCurrentWindow", GetCurrentWindow);
	Nan::SetMethod(GL, "getDrawableSize", GetDrawableSize);
	Nan::SetMethod(GL, "getSwapInterval", GetSwapInterval);

	// SDL_GLattr enum.
	Nan::Set(GL, Nan::New<String>("RED_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_RED_SIZE)));
	Nan::Set(GL, Nan::New<String>("GREEN_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_GREEN_SIZE)));
	Nan::Set(GL, Nan::New<String>("BLUE_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_BLUE_SIZE)));
	Nan::Set(GL, Nan::New<String>("ALPHA_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_ALPHA_SIZE)));
	Nan::Set(GL, Nan::New<String>("BUFFER_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_BUFFER_SIZE)));
	Nan::Set(GL, Nan::New<String>("DOUBLEBUFFER").ToLocalChecked(), Nan::New<Number>(SDL_GL_DOUBLEBUFFER)));
	Nan::Set(GL, Nan::New<String>("DEPTH_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_DEPTH_SIZE)));
	Nan::Set(GL, Nan::New<String>("STENCIL_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_STENCIL_SIZE)));
	Nan::Set(GL, Nan::New<String>("ACCUM_RED_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_ACCUM_RED_SIZE)));
	Nan::Set(GL, Nan::New<String>("ACCUM_GREEN_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_ACCUM_GREEN_SIZE)));
	Nan::Set(GL, Nan::New<String>("ACCUM_BLUE_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_ACCUM_BLUE_SIZE)));
	Nan::Set(GL, Nan::New<String>("ACCUM_ALPHA_SIZE").ToLocalChecked(), Nan::New<Number>(SDL_GL_ACCUM_ALPHA_SIZE)));
	Nan::Set(GL, Nan::New<String>("STEREO").ToLocalChecked(), Nan::New<Number>(SDL_GL_STEREO)));
	Nan::Set(GL, Nan::New<String>("MULTISAMPLEBUFFERS").ToLocalChecked(), Nan::New<Number>(SDL_GL_MULTISAMPLEBUFFERS)));
	Nan::Set(GL, Nan::New<String>("MULTISAMPLESAMPLES").ToLocalChecked(), Nan::New<Number>(SDL_GL_MULTISAMPLESAMPLES)));
	Nan::Set(GL, Nan::New<String>("ACCELERATED_VISUAL").ToLocalChecked(), Nan::New<Number>(SDL_GL_ACCELERATED_VISUAL)));
	Nan::Set(GL, Nan::New<String>("RETAINED_BACKING").ToLocalChecked(), Nan::New<Number>(SDL_GL_RETAINED_BACKING)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_MAJOR_VERSION").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_MAJOR_VERSION)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_MINOR_VERSION").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_MINOR_VERSION)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_EGL").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_EGL)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_FLAGS").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_FLAGS)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_PROFILE_MASK").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_PROFILE_MASK)));
	Nan::Set(GL, Nan::New<String>("SHARE_WITH_CURRENT_CONTEXT").ToLocalChecked(), Nan::New<Number>(SDL_GL_SHARE_WITH_CURRENT_CONTEXT)));
	Nan::Set(GL, Nan::New<String>("FRAMEBUFFER_SRGB_CAPABLE").ToLocalChecked(), Nan::New<Number>(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE)));
	// SDL_GLprofile enum.
	Nan::Set(GL, Nan::New<String>("CONTEXT_PROFILE_CORE").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_PROFILE_CORE)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_PROFILE_COMPATIBILITY").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY)));
	Nan::Set(GL, Nan::New<String>("PROFILE_ES").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_PROFILE_ES)));
	// SDL_GLcontextFlag enum.
	Nan::Set(GL, Nan::New<String>("CONTEXT_DEBUG_FLAG").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_DEBUG_FLAG)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_FORWARD_COMPATIBLE_FLAG").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_ROBUST_ACCESS_FLAG").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG)));
	Nan::Set(GL, Nan::New<String>("CONTEXT_RESET_ISOLATION_FLAG").ToLocalChecked(), Nan::New<Number>(SDL_GL_CONTEXT_RESET_ISOLATION_FLAG)));
}

Nan::Persistent<FunctionTemplate> sdl::gl::ContextWrapper::wrap_template_;

sdl::gl::ContextWrapper::ContextWrapper() {
}
sdl::gl::ContextWrapper::ContextWrapper(Handle<Object> obj) {
	Wrap(obj);
}
sdl::gl::ContextWrapper::~ContextWrapper() {
	if(NULL != context_) {
		SDL_GL_DeleteContext(context_);
	}
}

void sdl::gl::ContextWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("ContextWrapper"));
}

NAN_METHOD(sdl::gl::ContextWrapper::New) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected new sdl.ContextWrapper(Window)")));
	}

	WindowWrapper* window = Nan::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(info[0]));
	SDL_GLContext context = SDL_GL_CreateContext(window->window_);
	if(NULL == context) {
		return ThrowSDLException(__func__);
	}

	ContextWrapper* obj = new ContextWrapper();
	obj->context_ = context;
	obj->Wrap(info.This());
	return info.This();
}

NAN_METHOD(sdl::gl::BindTexture) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Must create a Context with the new operator.")));
	}

	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected BindTexture(Texture)")));
	}

	TextureWrapper* obj = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info[0]));
	float texw, texh;
	int err = SDL_GL_BindTexture(obj->texture_, &texw, &texh);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(texw));
	Nan::Set(ret, 1, Nan::New<Number>(texh));
	return scope.Close(ret);
}
NAN_METHOD(sdl::gl::UnbindTexture) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected BindTexture(Texture)")));
	}

	TextureWrapper* obj = Nan::ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(info[0]));
	int err = SDL_GL_UnbindTexture(obj->texture_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::gl::ExtensionSupported) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected ExtensionSupported(String)")));
	}

	String::Utf8Value name(info[0]);
	SDL_bool ret = SDL_GL_ExtensionSupported(*name);
	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}

NAN_METHOD(sdl::gl::LoadLibrary) {
	HandleScope scope;

	int err;
	if(info[0]->IsUndefined()) {
		err = SDL_GL_LoadLibrary(NULL);
	}
	else {
		String::Utf8Value name(info[0]);
		err = SDL_GL_LoadLibrary(*name);
	}
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::gl::UnloadLibrary) {
	HandleScope scope;

	SDL_GL_UnloadLibrary();
	
	return Undefined();
}

NAN_METHOD(sdl::gl::SetAttribute) {
	HandleScope scope;

	if (!(info.Length() == 2 && info[0]->IsNumber() && info[1]->IsNumber())) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetAttribute(Number, Number)")));
	}

	int attr = info[0]->Int32Value();
	int value = info[1]->Int32Value();

	if (SDL_GL_SetAttribute((SDL_GLattr)attr, value)) return ThrowSDLException(__func__);
	return Undefined();
}
NAN_METHOD(sdl::gl::MakeCurrent) {
	HandleScope scope;

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected MakeCurrent(Window, GLContext)")));
	}

	WindowWrapper* window = Nan::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(info[0]));
	ContextWrapper* context = Nan::ObjectWrap::Unwrap<ContextWrapper>(Handle<Object>::Cast(info[1]));
	int err = SDL_GL_MakeCurrent(window->window_, context->context_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
NAN_METHOD(sdl::gl::SetSwapInterval) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Exception SetSwapInterval(Number)")));
	}

	int interval = info[0]->Int32Value();
	int err = SDL_GL_SetSwapInterval(interval);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::gl::GetAttribute) {
	HandleScope scope;

	if (!(info.Length() == 1 && info[0]->IsNumber())) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected GetAttribute(Number)")));
	}

	int attr = info[0]->Int32Value();
	int value;

	if (SDL_GL_GetAttribute((SDL_GLattr)attr, &value)) return ThrowSDLException(__func__);

	return Nan::New<Number>(value);
}
NAN_METHOD(sdl::gl::GetCurrentContext) {
	HandleScope scope;

	SDL_GLContext context = SDL_GL_GetCurrentContext();
	if(NULL == context) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> newObj = Nan::New<Object>();
	ContextWrapper* wrap = new ContextWrapper(newObj);
	wrap->context_ = context;
	return scope.Close(newObj);
}
NAN_METHOD(sdl::gl::GetCurrentWindow) {
	HandleScope scope;

	SDL_Window* window = SDL_GL_GetCurrentWindow();
	if(NULL == window) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> newObj = Nan::New<Object>();
	WindowWrapper* wrap = new WindowWrapper(newObj);
	wrap->window_ = window;
	return scope.Close(newObj);
}
// TODO: Migrate to WindowWrapper object.
NAN_METHOD(sdl::gl::GetDrawableSize) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetDrawableSize(Window)")));
	}

	WindowWrapper* wrap = Nan::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(info[0]));
	int w, h;
	SDL_GL_GetDrawableSize(wrap->window_, &w, &h);

	Handle<Array> ret = Array::New(2);
	Nan::Set(ret, 0, Nan::New<Number>(w));
	Nan::Set(ret, 0, Nan::New<Number>(h));
	return scope.Close(ret);
}
// TODO: Handle this somehow?
// Theoretically handleable with Handle<External>, but I don't see any use in wrapping a function
// pointer for Javascript. Better off loading addresses invisibly when SDL/OpenGl gets initialized
// and providing an API to access the functions.
// NAN_METHOD(sdl::gl::GetProcAddress) {
// }
NAN_METHOD(sdl::gl::GetSwapInterval) {
	HandleScope scope;

	int interval = SDL_GL_GetSwapInterval();
	return scope.Close(Nan::New<Number>(interval));
}


