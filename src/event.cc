#include "event.h"
#include "helpers.h"
#include "container.h"

#include <string>

using namespace v8;
using namespace node;


void sdl::event::Init(Handle<Object> exports) {
	Nan::SetMethod(exports, "eventState", EventState);

	Nan::SetMethod(exports, "flushEvent", FlushEvent);
	Nan::SetMethod(exports, "flushEvents", FlushEvents);

	Nan::SetMethod(exports, "getNumtouchDevices", GetNumTouchDevices);
	Nan::SetMethod(exports, "getNumTouchFingers", GetNumTouchFingers);
	Nan::SetMethod(exports, "getTouchDevice", GetTouchDevice);
	Nan::SetMethod(exports, "getTouchFinger", GetTouchFinger);
	Nan::SetMethod(exports, "recordGesture", RecordGesture);

	Nan::SetMethod(exports, "hasEvent", HasEvent);
	Nan::SetMethod(exports, "hasEvents", HasEvents);

	Nan::SetMethod(exports, "waitEvent", sdl::WaitEvent);
	Nan::SetMethod(exports, "waitEventTimeout", sdl::WaitEventTimeout);
	Nan::SetMethod(exports, "pollEvent", sdl::PollEvent);

	Nan::SetMethod(exports, "quitRequested", QuitRequested);
}

// TODO: Implement these in a way that will be able to call Javascript functions on the
//       main thread. (possibly create a custom implementation?) The callback to SDL_AddEventWatch,
//       per the SDL documentation, is not guaranteed to be on the same thread.
// NAN_METHOD(sdl::AddEventWatch) {
// 	HandleScope scope;
// 	return Undefined();
// }
// NAN_METHOD(sdl::DelEventWatch) {
// 	HandleScope scope;
// 	return Undefined();
// }
NAN_METHOD(sdl::EventState) {
	HandleScope scope;

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected EventState(Number, Number)")));
	}

	int type = info[0]->Int32Value();
	int state = info[1]->Int32Value();
	int8_t ret = SDL_EventState(type, state);

	return scope.Close(Nan::New<Number>(ret));
}

// TODO: Implement these in a way that will be able to call Javascript functions on the
//       main thread. (possibly create a custom implementation?) The callback to SDL_SetEventFilter,
//       per the SDL documentation, is not guaranteed to be on the same thread.
// NAN_METHOD(sdl::FilterEvents) {
// 	HandleScope scope;
// 	return Undefined();
// }
// NAN_METHOD(sdl::GetEventFilter) {
// 	HandleScope scope;
// 	return Undefined();
// }
// NAN_METHOD(sdl::SetEventFilter) {
// 	HandleScope scope;
// 	return Undefined();
// }

NAN_METHOD(sdl::FlushEvent) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected FlushEvent(Number)")));
	}

	int type = info[0]->Int32Value();
	SDL_FlushEvent(type);

	return Undefined();
}
NAN_METHOD(sdl::FlushEvents) {
	HandleScope scope;

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected FlushEvent(Number, Number)")));
	}

	int typeMin = info[0]->Int32Value();
	int typeMax = info[1]->Int32Value();
	SDL_FlushEvents(typeMin, typeMax);

	return Undefined();
}

NAN_METHOD(sdl::GetNumTouchDevices) {
	HandleScope scope;

	int ret = SDL_GetNumTouchDevices();

	return scope.Close(Nan::New<Number>(ret));
}
NAN_METHOD(sdl::GetNumTouchFingers) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetNumTouchFingers(Number)")));
	}

	SDL_TouchID id = static_cast<SDL_TouchID>(info[0]->IntegerValue());
	int ret = SDL_GetNumTouchFingers(id);
	if(0 == ret) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(ret));
}
NAN_METHOD(sdl::GetTouchDevice) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetTouchDevice(Number)")));
	}

	int index = info[0]->Int32Value();
	SDL_TouchID device = SDL_GetTouchDevice(index);
	if(0 == device) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Nan::New<Number>(device));
}
NAN_METHOD(sdl::GetTouchFinger) {
	HandleScope scope;

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected GetTouchFinger(Number, Number)")));
	}

	SDL_TouchID id = static_cast<SDL_TouchID>(info[0]->IntegerValue());
	int index = info[1]->Int32Value();
	SDL_Finger* finger = SDL_GetTouchFinger(id, index);
	if(NULL == finger) {
		return ThrowSDLException(__func__);
	}

	Local<Object> toWrap = Nan::New<Object>();
	FingerWrapper* ret = new FingerWrapper(toWrap);
	ret->finger_ = finger;

	return scope.Close(toWrap);
}
NAN_METHOD(sdl::RecordGesture) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected RecordGesture(Number)")));
	}

	SDL_TouchID id = static_cast<SDL_TouchID>(info[0]->IntegerValue());
	int err = SDL_RecordGesture(id);
	if(0 == err) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

NAN_METHOD(sdl::HasEvent) {
	HandleScope scope;

	if(info.Length() < 1) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected HasEvent(Number)")));
	}

	int type = info[0]->Int32Value();
	SDL_bool ret = SDL_HasEvent(type);

	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}
NAN_METHOD(sdl::HasEvents) {
	HandleScope scope;

	if(info.Length() < 2) {
		return ThrowException(Exception::TypeError(
			Nan::New<String>("Invalid arguments: Expected HasEvents(Number, Number)")));
	}

	int typeMin = info[0]->Int32Value();
	int typeMax = info[1]->Int32Value();
	SDL_bool ret = SDL_HasEvents(typeMin, typeMax);

	return scope.Close(Nan::New<Boolean>(ret ? true : false));
}

// TODO: Implement these, along with RWOps wrapper.
//       (too lazy to implement RWOps right now)
// NAN_METHOD(sdl::LoadDollarTemplates) {
// 	HandleScope scope;

// 	return Undefined();
// }
// NAN_METHOD(sdl::SaveAllDollarTemplates) {
// 	HandleScope scope;

// 	return Undefined();
// }
// NAN_METHOD(sdl::SaveDollarTemplate) {
// 	HandleScope scope;

// 	return Undefined();
// }

NAN_METHOD(sdl::WaitEvent) {
	HandleScope scope;

	if (!(info.Length() == 1 && info[0]->IsFunction())) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected WaitEvent(Function)")));
	}

	SDL_Event e;
	int err = SDL_WaitEvent(&e);
	if(0 == err) {
		std::string err = "WaitEvent failed: ";
		err += SDL_GetError();
		return ThrowException(MakeSDLException(err.c_str()));
	}
	Handle<Value> argv[1];
	argv[0] = sdl::SDLEventToJavascriptObject(e);
	Handle<Function>::Cast(info[0])->Call(Context::GetCurrent()->Global(), 1, argv);
	return Undefined();
}

NAN_METHOD(sdl::WaitEventTimeout) {
	HandleScope scope;

	if(!(info.Length() == 2 && info[0]->IsFunction() && info[1]->IsNumber())) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected WaitEventTimeout(Function, Number)")));
	}

	SDL_Event e;
	int timeout = info[1]->Int32Value();
	int err = SDL_WaitEventTimeout(&e, timeout);
	if(0 == err) {
		std::string err = "WaitEventTimeout failed: ";
		err += SDL_GetError();
		return ThrowException(MakeSDLException(err.c_str()));
	}
	Handle<Value> argv[1];
	argv[0] = sdl::SDLEventToJavascriptObject(e);
	Handle<Function>::Cast(info[0])->Call(Context::GetCurrent()->Global(), 1, argv);
	return Undefined();
}

NAN_METHOD(sdl::PollEvent) {
	HandleScope scope;

	if (!(info.Length() == 0)) {
		return ThrowException(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected PollEvent()")));
	}

	SDL_Event event;
	if (!SDL_PollEvent(&event)) {
		return Undefined();
	}

	Local<Object> evt = SDLEventToJavascriptObject(event);
	return scope.Close(evt);
}
// TODO: Implement this function.
//		 This is a relatively complicated function which uses an array to put/get events into/from
//		 the event queue. The same functionality can be achieved using other functions, so I'll
// 		 deal with this later. (too lazy right now, again)
// NAN_METHOD(sdl::PeepEvents) {
// 	HandleScope scope;

// 	return Undefined();
// }
// TODO: Implement this function.
// 		 None of the functions which make pump useful are implemented, so there's no use in
// 		 exposing this function yet. (for example, filtering events of peeping events)
//		 Just use poll for now.
// NAN_METHOD(sdl::PumpEvents) {
// 	HandleScope scope;

// 	return Undefined();
// }
// TODO: Implement when events are refactored. Huge pain to convert from object to event right now,
//  	 similar effort to the current event to object conversion. (should just be a wrapper)
// NAN_METHOD(sdl::PushEvent) {
// 	HandleScope scope;

// 	if(info.Length() < 1) {
// 		return ThrowException(Exception::TypeError(
// 			Nan::New<String>("Invalid arguments: Expected PushEvent(Event)")));
// 	}

// 	return Undefined();
// }
// TODO: Implement when push is implemented. Can only be used when you can push a custom
// 		 event.
// NAN_METHOD(sdl::RegisterEvents) {
// 	HandleScope scope;

// 	return Undefined();
// }

NAN_METHOD(sdl::QuitRequested) {
	HandleScope scope;

	bool ret = SDL_QuitRequested();

	return scope.Close(Nan::New<Boolean>(ret));
}

// TODO: Improve on how events are created. Gotta be a better way than this...
Local<Object> SDLEventToJavascriptObject(const SDL_Event& event) {
	Local<Object> evt = Nan::New<Object>();

	switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_KEYDOWN ? "KEYDOWN" : "KEYUP")));
			Nan::Set(evt, Nan::New<String>("scancode").ToLocalChecked(), Nan::New<Number>(event.key.keysym.scancode)));
			Nan::Set(evt, Nan::New<String>("sym").ToLocalChecked(), Nan::New<Number>(event.key.keysym.sym)));
			Nan::Set(evt, Nan::New<String>("mod").ToLocalChecked(), Nan::New<Number>(event.key.keysym.mod)));
			break;
		case SDL_MOUSEMOTION:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("MOUSEMOTION")));
			Nan::Set(evt, Nan::New<String>("state").ToLocalChecked(), Nan::New<Number>(event.motion.state)));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.motion.which)));
			Nan::Set(evt, Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(event.motion.x)));
			Nan::Set(evt, Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(event.motion.y)));
			Nan::Set(evt, Nan::New<String>("xrel").ToLocalChecked(), Nan::New<Number>(event.motion.xrel)));
			Nan::Set(evt, Nan::New<String>("yrel").ToLocalChecked(), Nan::New<Number>(event.motion.yrel)));
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_MOUSEBUTTONDOWN ? "MOUSEBUTTONDOWN" : "MOUSEBUTTONUP")));
			Nan::Set(evt, Nan::New<String>("button").ToLocalChecked(), Nan::New<Number>(event.button.button)));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.button.which)));
			Nan::Set(evt, Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(event.button.x)));
			Nan::Set(evt, Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(event.button.y)));
			break;
		case SDL_JOYAXISMOTION:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYAXISMOTION")));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jaxis.which)));
			Nan::Set(evt, Nan::New<String>("axis").ToLocalChecked(), Nan::New<Number>(event.jaxis.axis)));
			Nan::Set(evt, Nan::New<String>("value").ToLocalChecked(), Nan::New<Number>(event.jaxis.value)));
			break;
		case SDL_JOYBALLMOTION:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYBALLMOTION")));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jball.which)));
			Nan::Set(evt, Nan::New<String>("ball").ToLocalChecked(), Nan::New<Number>(event.jball.ball)));
			Nan::Set(evt, Nan::New<String>("xrel").ToLocalChecked(), Nan::New<Number>(event.jball.xrel)));
			Nan::Set(evt, Nan::New<String>("yrel").ToLocalChecked(), Nan::New<Number>(event.jball.yrel)));
			break;
		case SDL_JOYHATMOTION:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYHATMOTION")));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jhat.which)));
			Nan::Set(evt, Nan::New<String>("hat").ToLocalChecked(), Nan::New<Number>(event.jhat.hat)));
			Nan::Set(evt, Nan::New<String>("value").ToLocalChecked(), Nan::New<Number>(event.jhat.value)));
			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_JOYBUTTONDOWN ? "JOYBUTTONDOWN" : "JOYBUTTONUP")));
			Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jbutton.which)));
			Nan::Set(evt, Nan::New<String>("button").ToLocalChecked(), Nan::New<Number>(event.jbutton.button)));
			break;
		case SDL_QUIT:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("QUIT")));
			break;
		default:
			Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("UNKNOWN")));
			Nan::Set(evt, Nan::New<String>("typeCode").ToLocalChecked(), Nan::New<Number>(event.type)));
			break;
	}

	return evt;
}
