#include "joystick.h"
#include "helpers.h"
#include "struct_wrappers.h"


using namespace v8;
using namespace node;


void sdl::joystick::Init(Handle<Object> exports) {
	JoystickWrapper::Init(exports);

	Nan::SetMethod(exports, "numJoysticks", NumJoysticks);

	Nan::SetMethod(exports, "joystickNameForIndex", JoystickNameForIndex);
	Nan::SetMethod(exports, "joystickGetDeviceGUID", JoystickGetDeviceGUID);
	Nan::SetMethod(exports, "joystickGetGUIDFromString", JoystickGetGUIDFromString);
	Nan::SetMethod(exports, "joystickGetGUIDString", JoystickGetGUIDString);

	Nan::SetMethod(exports, "joystickUpdate", JoystickUpdate);
	Nan::SetMethod(exports, "joystickEventState", JoystickEventState);
}

Nan::Persistent<FunctionTemplate> sdl::JoystickWrapper::wrap_template_;

sdl::JoystickWrapper::JoystickWrapper() {
}
sdl::JoystickWrapper::JoystickWrapper(bool owned) {
	owned_ = owned;
}
sdl::JoystickWrapper::~JoystickWrapper() {
	if(NULL != joystick_ && !owned_) {
		SDL_JoystickClose(joystick_);
	}
}

void sdl::JoystickWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("JoystickWrapper"));

	Nan::SetPrototypeMethod(wrap_template_, "getAttached", GetAttached);
	Nan::SetPrototypeMethod(wrap_template_, "getAxis", GetAxis);
	Nan::SetPrototypeMethod(wrap_template_, "getBall", GetBall);
	Nan::SetPrototypeMethod(wrap_template_, "getButton", GetButton);
	Nan::SetPrototypeMethod(wrap_template_, "getGUID", GetGUID);
	Nan::SetPrototypeMethod(wrap_template_, "getHat", GetHat);
	Nan::SetPrototypeMethod(wrap_template_, "getName", GetName);
	Nan::SetPrototypeMethod(wrap_template_, "getNumAxes", GetNumAxes);
	Nan::SetPrototypeMethod(wrap_template_, "getNumButtons", GetNumButtons);
	Nan::SetPrototypeMethod(wrap_template_, "getNumBalls", GetNumBalls);
	Nan::SetPrototypeMethod(wrap_template_, "getNumHats", GetNumHats);

	Nan::Set(exports, Nan::New<String>("Joystick"), wrap_template_->GetFunction());
}
NAN_METHOD(sdl::JoystickWrapper::New) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Must create an sdl.Joystick with the new operator.")));
	}

	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected new sdl.Joystick(Number)")));
	}

	if(info[0]->IsExternal()) {
		JoystickWrapper* wrap;
		if(info.Length() > 1) {
			wrap = new JoystickWrapper(info[1]->BooleanValue());
		}
		else {
			wrap = new JoystickWrapper();
		}
		wrap->joystick_ = static_cast<SDL_Joystick*>(Handle<External>::Cast(info[0])->Value());
		wrap->Wrap(info.This());
	}
	else {
		int index = info[0]->Int32Value();
		SDL_Joystick* joystick = SDL_JoystickOpen(index);
		if(NULL == joystick) {
			return ThrowSDLException(__func__);
		}

		JoystickWrapper* wrap = new JoystickWrapper();
		wrap->joystick_ = joystick;
		wrap->Wrap(info.This());
	}

	return info.This();
}

NAN_METHOD(sdl::JoystickWrapper::GetAttached) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	SDL_bool attached = SDL_JoystickGetAttached(wrap->joystick_);

	return scope.Close(Nan::New<Boolean>(attached ? true : false));
}
NAN_METHOD(sdl::JoystickWrapper::GetAxis) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetAxis(Number)")));
	}

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int axis = info[0]->Int32Value();
	int16_t position = SDL_JoystickGetAxis(wrap->joystick_, axis);
	if(0 == position) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(position));
}
NAN_METHOD(sdl::JoystickWrapper::GetBall) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetBall(Number)")));
	}

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int ball = info[0]->Int32Value();
	int dx, dy;
	int err = SDL_JoystickGetBall(wrap->joystick_, ball, &dx, &dy);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Nan::New<Object>();
	Nan::Set(ret, Nan::New<String>("dx"), Nan::New<Number>(dx));
	Nan::Set(ret, Nan::New<String>("dy"), Nan::New<Number>(dy));

	return scope.Close(ret);
}
NAN_METHOD(sdl::JoystickWrapper::GetButton) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetButton(Number)")));
	}

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int button = info[0]->Int32Value();
	uint8_t ret = SDL_JoystickGetButton(wrap->joystick_, button);

	return scope.Close(Nan::New<Boolean>(ret == 1 ? true : false));
}
NAN_METHOD(sdl::JoystickWrapper::GetGUID) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	SDL_JoystickGUID guid = SDL_JoystickGetGUID(wrap->joystick_);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		Nan::Set(ret, i, Nan::New<Number>(guid.data[i]));
	}

	return scope.Close(ret);
}
NAN_METHOD(sdl::JoystickWrapper::GetHat) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetHat(Number)")));
	}

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int hat = info[0]->Int32Value();
	uint8_t ret = SDL_JoystickGetHat(wrap->joystick_, hat);

	return scope.Close(Nan::New<Number>(ret));
}

NAN_METHOD(sdl::JoystickWrapper::GetName) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	const char* name = SDL_JoystickName(wrap->joystick_);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(name));
}
NAN_METHOD(sdl::JoystickWrapper::GetNumAxes) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int axes = SDL_JoystickNumAxes(wrap->joystick_);
	if(axes < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(axes));
}
NAN_METHOD(sdl::JoystickWrapper::GetNumButtons) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int buttons = SDL_JoystickNumButtons(wrap->joystick_);
	if(buttons < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(buttons));
}
NAN_METHOD(sdl::JoystickWrapper::GetNumBalls) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int balls = SDL_JoystickNumBalls(wrap->joystick_);
	if(balls < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(balls));
}
NAN_METHOD(sdl::JoystickWrapper::GetNumHats) {
	HandleScope scope;

	JoystickWrapper* wrap = Nan::ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(info.This()));
	int hats = SDL_JoystickNumHats(wrap->joystick_);
	if(hats < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(hats));
}

NAN_METHOD(sdl::NumJoysticks) {
	HandleScope scope;

	return scope.Close(Nan::New<Number>(SDL_NumJoysticks()));
}

NAN_METHOD(sdl::JoystickNameForIndex) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected JoystickNameForIndex(Number)")));
	}

	int index = info[0]->Int32Value();
	const char* name = SDL_JoystickNameForIndex(index);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(name));
}

NAN_METHOD(sdl::JoystickGetDeviceGUID) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected JoystickGetDeviceGUID(Number)")));
	}

	int index = info[0]->Int32Value();
	SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(index);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		Nan::Set(ret, i, Nan::New<Number>(guid.data[i]));
	}

	return scope.Close(ret);
}
NAN_METHOD(sdl::JoystickGetGUIDFromString) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected JoystickGetGUIDFromString(String)")));
	}

	String::Utf8Value pchGuid(info[0]);
	SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(*pchGuid);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 uint8_t elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		Nan::Set(ret, i, Nan::New<Number>(guid.data[i]));
	}

	return scope.Close(ret);
}
NAN_METHOD(sdl::JoystickGetGUIDString) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected JoystickGetGUIDString(String)")));
	}

	Handle<Array> guidArr = Handle<Array>::Cast(info[0]);
	SDL_JoystickGUID guid;
	for(int i = 0; i < 16; i++) {
		guid.data[i] = static_cast<uint8_t>(guidArr->Get(i)->Uint32Value());
	}
	// Hopefully 200 characters is enough to hold the string. Should this be a higher number?
	char pszGuid[200];
	SDL_JoystickGetGUIDString(guid, pszGuid, 200);

	return scope.Close(Nan::New<String>(pszGuid));
}

NAN_METHOD(sdl::JoystickUpdate) {
	HandleScope scope;

	SDL_JoystickUpdate();

	return Undefined();
}

NAN_METHOD(sdl::JoystickEventState) {
	HandleScope scope;

	int state;
	if (info.Length() == 0) {
		state = SDL_QUERY;
	} else {
		if (!(info.Length() == 1 && info[0]->IsBoolean())) {
			return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected JoystickEventState([Boolean])")));
		}
		state = info[0]->BooleanValue() ? SDL_ENABLE : SDL_IGNORE;
	}
	return Nan::New<Boolean>(SDL_JoystickEventState(state));
}
