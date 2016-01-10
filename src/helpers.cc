#include <v8.h>
#include <nan.h>
#include <node.h>
#include <node_buffer.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "helpers.h"
#include "struct_wrappers.h"

using namespace v8;
using namespace node;

namespace sdl {

    // Helper for formatting error exceptions
  Handle<Value> ThrowSDLException(const char* name) {
    v8::Isolate* isolate  = v8::Isolate::New();
    v8::Local<v8::Value> exception = MakeSDLException(name);
    return isolate->ThrowException(exception);
    // return v8::Isolate::ThrowException();
  }

  Local<Value> MakeSDLException(const char* name) {
    return Exception::Error(String::Concat(
      String::Concat(Nan::New<String>(name).ToLocalChecked(), Nan::New<String>(": ").ToLocalChecked()),
      Nan::New<String>(SDL_GetError()).ToLocalChecked()
      ));
  }

  // char* BufferData(Nan::MaybeLocal<v8::Object> *b) {
  //   return Buffer::Data(b->handle_);
  // }

  // size_t BufferLength(Nan::MaybeLocal<v8::Object> *b) {
  //   return Buffer::Length(b->handle_);
  // }

  // char* BufferData(Local<Object> buf_obj) {
  //   return Buffer::Data(buf_obj);
  // }

  // size_t BufferLength(Local<Object> buf_obj) {
  //   return Buffer::Length(buf_obj);
  // }

  Local<Object> SDLEventToJavascriptObject(const SDL_Event& event) {
    Local<Object> evt = Nan::New<Object>();

    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_KEYDOWN ? "KEYDOWN" : "KEYUP").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("scancode").ToLocalChecked(), Nan::New<Number>(event.key.keysym.scancode));
        Nan::Set(evt, Nan::New<String>("sym").ToLocalChecked(), Nan::New<Number>(event.key.keysym.sym));
        Nan::Set(evt, Nan::New<String>("mod").ToLocalChecked(), Nan::New<Number>(event.key.keysym.mod));
        Nan::Set(evt, Nan::New<String>("repeat").ToLocalChecked(), Nan::New<Boolean>(event.key.repeat > 0 ? true : false));
      break;
      case SDL_MOUSEMOTION:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("MOUSEMOTION").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("state").ToLocalChecked(), Nan::New<Number>(event.motion.state));
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.motion.which));
        Nan::Set(evt, Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(event.motion.x));
        Nan::Set(evt, Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(event.motion.y));
        Nan::Set(evt, Nan::New<String>("xrel").ToLocalChecked(), Nan::New<Number>(event.motion.xrel));
        Nan::Set(evt, Nan::New<String>("yrel").ToLocalChecked(), Nan::New<Number>(event.motion.yrel));
      break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_MOUSEBUTTONDOWN ? "MOUSEBUTTONDOWN" : "MOUSEBUTTONUP").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("button").ToLocalChecked(), Nan::New<Number>(event.button.button));
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.button.which));
        Nan::Set(evt, Nan::New<String>("x").ToLocalChecked(), Nan::New<Number>(event.button.x));
        Nan::Set(evt, Nan::New<String>("y").ToLocalChecked(), Nan::New<Number>(event.button.y));
      break;
      case SDL_JOYAXISMOTION:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYAXISMOTION").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jaxis.which));
        Nan::Set(evt, Nan::New<String>("axis").ToLocalChecked(), Nan::New<Number>(event.jaxis.axis));
        Nan::Set(evt, Nan::New<String>("value").ToLocalChecked(), Nan::New<Number>(event.jaxis.value));
      break;
      case SDL_JOYBALLMOTION:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYBALLMOTION").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jball.which));
        Nan::Set(evt, Nan::New<String>("ball").ToLocalChecked(), Nan::New<Number>(event.jball.ball));
        Nan::Set(evt, Nan::New<String>("xrel").ToLocalChecked(), Nan::New<Number>(event.jball.xrel));
        Nan::Set(evt, Nan::New<String>("yrel").ToLocalChecked(), Nan::New<Number>(event.jball.yrel));
      break;
      case SDL_JOYHATMOTION:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("JOYHATMOTION").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jhat.which));
        Nan::Set(evt, Nan::New<String>("hat").ToLocalChecked(), Nan::New<Number>(event.jhat.hat));
        Nan::Set(evt, Nan::New<String>("value").ToLocalChecked(), Nan::New<Number>(event.jhat.value));
      break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>(event.type == SDL_JOYBUTTONDOWN ? "JOYBUTTONDOWN" : "JOYBUTTONUP").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("which").ToLocalChecked(), Nan::New<Number>(event.jbutton.which));
        Nan::Set(evt, Nan::New<String>("button").ToLocalChecked(), Nan::New<Number>(event.jbutton.button));
      break;
      case SDL_QUIT:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("QUIT").ToLocalChecked());
      break;
      default:
        Nan::Set(evt, Nan::New<String>("type").ToLocalChecked(), Nan::New<String>("UNKNOWN").ToLocalChecked());
        Nan::Set(evt, Nan::New<String>("typeCode").ToLocalChecked(), Nan::New<Number>(event.type));
      break;
    }

    return evt;
  }

  Local<Object> SDLDisplayModeToJavascriptObject(const SDL_DisplayMode& mode) {
    Local<Object> jsMode = Nan::New<Object>();
    Nan::Set(jsMode, Nan::New<String>("format").ToLocalChecked(), Nan::New<Number>(mode.format));
    Nan::Set(jsMode, Nan::New<String>("w").ToLocalChecked(), Nan::New<Number>(mode.w));
    Nan::Set(jsMode, Nan::New<String>("h").ToLocalChecked(), Nan::New<Number>(mode.h));
    Nan::Set(jsMode, Nan::New<String>("refreshRate").ToLocalChecked(), Nan::New<Number>(mode.refresh_rate));
    return jsMode;
  }

} // node_sdl
