#include "controller.h"
#include "joystick.h"
#include "helpers.h"

using namespace v8;
using namespace node;


static Handle<Object> ControllerButtonBindToObject(SDL_GameControllerButtonBind bind) {
	Handle<Object> ret = Nan::New<Object>();
	Nan::Set(ret, Nan::New<String>("type"), Nan::New<Number>(bind.bindType));
	switch(bind.bindType) {
	case SDL_CONTROLLER_BINDTYPE_NONE:
		// Nothing to do here. Just wanted to shut the compiler up :)
		break;
	case SDL_CONTROLLER_BINDTYPE_BUTTON:
		Nan::Set(ret, Nan::New<String>("button"), Nan::New<Number>(bind.value.button));
		break;
	case SDL_CONTROLLER_BINDTYPE_AXIS:
		Nan::Set(ret, Nan::New<String>("axis"), Nan::New<Number>(bind.value.axis));
		break;
	case SDL_CONTROLLER_BINDTYPE_HAT:
		Handle<Object> hat = Nan::New<Object>();
		Nan::Set(hat, Nan::New<String>("hat"), Nan::New<Number>(bind.value.hat.hat));
		Nan::Set(hat, Nan::New<String>("hatMask"), Nan::New<Number>(bind.value.hat.hat_mask));
		Nan::Set(ret, Nan::New<String>("hat"), hat);
		break;
	}
	return ret;
}

void sdl::controller::Init(Handle<Object> exports) {
	GameControllerWrapper::Init(exports);

	Nan::SetMethod(exports, "addMapping", AddMapping);
	Nan::SetMethod(exports, "addMappingsFromFile", AddMappingsFromFile);

	Nan::SetMethod(exports, "mappingForGuid", MappingForGUID);
	Nan::SetMethod(exports, "nameForIndex", NameForIndex);

	Nan::SetMethod(exports, "eventState", EventState);
	Nan::SetMethod(exports, "getAxisFromString", GetAxisFromString);
	Nan::SetMethod(exports, "getButtonFromString", GetButtonFromString);

	Handle<Object> BINDTYPE = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("BINDTYPE"), BINDTYPE);
	Nan::Set(BINDTYPE, Nan::New<String>("NONE"), Nan::New<Number>(SDL_CONTROLLER_BINDTYPE_NONE));
	Nan::Set(BINDTYPE, Nan::New<String>("BUTTON"), Nan::New<Number>(SDL_CONTROLLER_BINDTYPE_BUTTON));
	Nan::Set(BINDTYPE, Nan::New<String>("AXIS"), Nan::New<Number>(SDL_CONTROLLER_BINDTYPE_AXIS));
	Nan::Set(BINDTYPE, Nan::New<String>("HAT"), Nan::New<Number>(SDL_CONTROLLER_BINDTYPE_HAT));

	Handle<Object> AXIS = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("AXIS"), AXIS);
	Nan::Set(AXIS, Nan::New<String>("INVALID"), Nan::New<Number>(SDL_CONTROLLER_AXIS_INVALID));
	Nan::Set(AXIS, Nan::New<String>("LEFTX"), Nan::New<Number>(SDL_CONTROLLER_AXIS_LEFTX));
	Nan::Set(AXIS, Nan::New<String>("LEFTY"), Nan::New<Number>(SDL_CONTROLLER_AXIS_LEFTY));
	Nan::Set(AXIS, Nan::New<String>("RIGHTX"), Nan::New<Number>(SDL_CONTROLLER_AXIS_RIGHTX));
	Nan::Set(AXIS, Nan::New<String>("RIGHTY"), Nan::New<Number>(SDL_CONTROLLER_AXIS_RIGHTY));
	Nan::Set(AXIS, Nan::New<String>("TRIGGERLEFT"), Nan::New<Number>(SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	Nan::Set(AXIS, Nan::New<String>("TRIGGERRIGHT"), Nan::New<Number>(SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
	Nan::Set(AXIS, Nan::New<String>("MAX"), Nan::New<Number>(SDL_CONTROLLER_AXIS_MAX));

	Handle<Object> BUTTON = Nan::New<Object>();
	Nan::Set(exports, Nan::New<String>("BUTTON"), BUTTON);
	Nan::Set(BUTTON, Nan::New<String>("INVALID"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_INVALID));
	Nan::Set(BUTTON, Nan::New<String>("A"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_A));
	Nan::Set(BUTTON, Nan::New<String>("B"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_B));
	Nan::Set(BUTTON, Nan::New<String>("X"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_X));
	Nan::Set(BUTTON, Nan::New<String>("Y"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_Y));
	Nan::Set(BUTTON, Nan::New<String>("BACK"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_BACK));
	Nan::Set(BUTTON, Nan::New<String>("GUIDE"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_GUIDE));
	Nan::Set(BUTTON, Nan::New<String>("START"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_START));
	Nan::Set(BUTTON, Nan::New<String>("LEFTSTICK"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_LEFTSTICK));
	Nan::Set(BUTTON, Nan::New<String>("RIGHTSTICK"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_RIGHTSTICK));
	Nan::Set(BUTTON, Nan::New<String>("LEFTSHOULDER"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
	Nan::Set(BUTTON, Nan::New<String>("RIGHTSHOULDER"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
	Nan::Set(BUTTON, Nan::New<String>("DPAD_UP"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_DPAD_UP));
	Nan::Set(BUTTON, Nan::New<String>("DPAD_DOWN"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
	Nan::Set(BUTTON, Nan::New<String>("DPAD_LEFT"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
	Nan::Set(BUTTON, Nan::New<String>("DPAD_RIGHT"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
	Nan::Set(BUTTON, Nan::New<String>("MAX"), Nan::New<Number>(SDL_CONTROLLER_BUTTON_MAX));
}

Nan::Persistent<FunctionTemplate> sdl::controller::GameControllerWrapper::wrap_template_;

sdl::controller::GameControllerWrapper::GameControllerWrapper() {
}
sdl::controller::GameControllerWrapper::~GameControllerWrapper() {
	if(NULL != controller_) {
		SDL_GameControllerClose(controller_);
	}
}

void sdl::controller::GameControllerWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(Nan::New<String>("GameControllerWrapper"));

	Nan::SetPrototypeMethod(wrap_template_, "getAttached", GetAttached);
	Nan::SetPrototypeMethod(wrap_template_, "getAxis", GetAxis);
	Nan::SetPrototypeMethod(wrap_template_, "getBindForAxis", GetBindForAxis);
	Nan::SetPrototypeMethod(wrap_template_, "getBindForButton", GetBindForButton);
	Nan::SetPrototypeMethod(wrap_template_, "getButton", GetButton);
	Nan::SetPrototypeMethod(wrap_template_, "getJoystick", GetJoystick);
	Nan::SetPrototypeMethod(wrap_template_, "getStringForAxis", GetStringForAxis);
	Nan::SetPrototypeMethod(wrap_template_, "getStringForButton", GetStringForButton);
	Nan::SetPrototypeMethod(wrap_template_, "getMapping", GetMapping);
  	Nan::SetPrototypeMethod(wrap_template_, "getName", GetName);

	Nan::Set(exports, Nan::New<String>("GameController"), wrap_template_->GetFunction());
}

NAN_METHOD(sdl::controller::GameControllerWrapper::New) {
	if(!info.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Must create a GameController with the new operator.")));
	}

	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Exepcted new sdl.GameController(Number)")));
	}

	int index = info[0]->Int32Value();
	SDL_GameController* controller = SDL_GameControllerOpen(index);
	if(NULL == controller) {
		return ThrowSDLException(__func__);
	}

	GameControllerWrapper* wrap = new GameControllerWrapper();
	wrap->controller_ = controller;
	wrap->Wrap(info.This());

	return info.This();
}

NAN_METHOD(sdl::controller::GameControllerWrapper::GetAttached) {
	HandleScope scope;

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_bool ret = SDL_GameControllerGetAttached(wrap->controller_);

	return scope.Close(Nan::New<Boolean>(ret == SDL_TRUE ? true : false));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetAxis) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetAxis(Number)")));
	}

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(info[0]->Int32Value());
	int16_t ret = SDL_GameControllerGetAxis(wrap->controller_, axis);

	return scope.Close(Nan::New<Number>(ret));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetBindForAxis) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetBindForAxis(Number)")));
	}

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(info[0]->Int32Value());
	SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForAxis(wrap->controller_, axis);

	return scope.Close(ControllerButtonBindToObject(bind));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetBindForButton) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetBindForButton(Number)")));
	}

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(info[0]->Int32Value());
	SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForButton(wrap->controller_, button);

	return scope.Close(ControllerButtonBindToObject(bind));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetButton) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetButton(Number)")));
	}

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(info[0]->Int32Value());
	uint8_t ret = SDL_GameControllerGetButton(wrap->controller_, button);

	return scope.Close(Nan::New<Boolean>(ret == 1 ? true : false));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetJoystick) {
	HandleScope scope;

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	SDL_Joystick* joystick = SDL_GameControllerGetJoystick(wrap->controller_);

	Handle<Value> arg = External::New(joystick);
	Handle<Value> argv[] = {arg, Nan::New<Boolean>(false)};
	return scope.Close(JoystickWrapper::wrap_template_->GetFunction()->NewInstance(2, argv));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetMapping) {
	HandleScope scope;

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	char* mapping = SDL_GameControllerMapping(wrap->controller_);
	if(NULL == mapping) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(mapping));
}
NAN_METHOD(sdl::controller::GameControllerWrapper::GetName) {
	HandleScope scope;

	GameControllerWrapper* wrap = Nan::ObjectWrap::Unwrap<GameControllerWrapper>(info.This());
	const char* name = SDL_GameControllerName(wrap->controller_);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(name));
}

NAN_METHOD(sdl::controller::AddMapping) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected AddMapping(String)")));
	}

	String::Utf8Value mapping(info[0]);
	int err = SDL_GameControllerAddMapping(*mapping);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(err));
}
NAN_METHOD(sdl::controller::AddMappingsFromFile) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected AddMappingFromFile(String)")));
	}

	String::Utf8Value file(info[0]);
	int err = SDL_GameControllerAddMappingsFromFile(*file);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(err));
}

NAN_METHOD(sdl::controller::MappingForGUID) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected MappingForGUID(String)")));
	}

	String::Utf8Value jsGuid(info[0]);
	SDL_JoystickGUID guid;
	for(int i = 0; i < 16; i++) {
		guid.data[i] = static_cast<uint8_t>((*jsGuid)[i]);
	}
	char* mapping = SDL_GameControllerMappingForGUID(guid);
	if(NULL == mapping) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(mapping));
}
NAN_METHOD(sdl::controller::NameForIndex) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected NameForIndex(Number)")));
	}

	int index = info[0]->Int32Value();
	const char* name = SDL_GameControllerNameForIndex(index);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<String>(name));
}

NAN_METHOD(sdl::controller::EventState) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected EventState(Number)")));
	}

	int state = info[0]->Int32Value();
	int ret = SDL_GameControllerEventState(state);

	return scope.Close(Nan::New<Number>(ret));
}
NAN_METHOD(sdl::controller::GetAxisFromString) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetAxisFromString(String)")));
	}

	String::Utf8Value pchString(info[0]);
	SDL_GameControllerAxis axis = SDL_GameControllerGetAxisFromString(*pchString);

	return scope.Close(Nan::New<Number>(axis));
}
NAN_METHOD(sdl::controller::GetButtonFromString) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetButtonFromString(String)")));
	}

	String::Utf8Value pchString(info[0]);
	SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString(*pchString);

	return scope.Close(Nan::New<Number>(button));
}
NAN_METHOD(sdl::controller::GetStringForAxis) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetStringForAxis(Number)")));
	}

	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(info[0]->Int32Value());
	const char* axisString = SDL_GameControllerGetStringForAxis(axis);

	return scope.Close(Nan::New<String>(axisString));
}
NAN_METHOD(sdl::controller::GetStringForButton) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetStringForButton(Number)")));
	}

	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(info[0]->Int32Value());
	const char* buttonString = SDL_GameControllerGetStringForButton(button);

	return scope.Close(Nan::New<String>(buttonString));
}

NAN_METHOD(Update) {
	HandleScope scope;

	SDL_GameControllerUpdate();

	return Undefined();
}
NAN_METHOD(IsController) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected IsController(Number)")));
	}

	int index = info[0]->Int32Value();
	SDL_bool ret = SDL_IsGameController(index);

	return scope.Close(Nan::New<Boolean>(ret == SDL_TRUE ? true : false));
}
