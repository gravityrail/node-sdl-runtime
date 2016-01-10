#ifndef NODEJS_EVENT_H
#define NODEJS_EVENT_H

#include <v8.h>
#include <node.h>
#include "SDL.h"


namespace sdl {
	namespace event {
		void Init(v8::Handle<v8::Object> exports);
	}

	// NAN_METHOD(AddEventWatch);
	// NAN_METHOD(DelEventWatch);
	NAN_METHOD(EventState);

	// NAN_METHOD(FilterEvents);
	// NAN_METHOD(GetEventFilter);
	// NAN_METHOD(SetEventFilter);

	NAN_METHOD(FlushEvent);
	NAN_METHOD(FlushEvents);

	NAN_METHOD(GetNumTouchDevices);
	NAN_METHOD(GetNumTouchFingers);
	NAN_METHOD(GetTouchDevice);
	NAN_METHOD(GetTouchFinger);
	NAN_METHOD(RecordGesture);

	NAN_METHOD(HasEvent);
	NAN_METHOD(HasEvents);

	// NAN_METHOD(LoadDollarTemplates);
	// NAN_METHOD(SaveAllDollarTemplates);
	// NAN_METHOD(SaveDollarTemplate);

	NAN_METHOD(WaitEvent);
	NAN_METHOD(WaitEventTimeout);
	NAN_METHOD(PollEvent);
	// NAN_METHOD(PeepEvents);
	// NAN_METHOD(PumpEvents);
	// NAN_METHOD(PushEvent);
	// NAN_METHOD(RegisterEvents);

	NAN_METHOD(QuitRequested);

	v8::Local<v8::Object> SDLEventToJavascriptObject(const SDL_Event& event);
}

#endif
