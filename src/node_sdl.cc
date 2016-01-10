#ifdef __APPLE__
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif

#include "SDL.h"
#include "SDL_revision.h"
#include "node_sdl.h"
#include "struct_wrappers.h"
#include "window.h"
#include "texture.h"
#include "surface.h"
#include "container.h"
#include "sdl_gl.h"
#include "event.h"
#include "key.h"
#include "mouse.h"
#include "font.h"
#include "joystick.h"
#include "controller.h"
#include "helpers.h"
#include <v8.h>
#include <string>
#include <iostream>
#include <ctime>
#include <sys/time.h>

using namespace v8;

static std::map<uint32_t, std::string> event_type_to_string_;
static std::map<SDL_WindowEventID, std::string> window_event_to_string_;

extern "C" void
init(Handle<Object> target)
{
// #ifdef __APPLE__
//   // on the mac it is necessary to create to call [NSApplication sharedApplication]
//   // before we can create a rendering window
//   objc_msgSend(objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
// #endif
  // std::cout << "Starting init." << std::endl;

  // Initialize the SDL event type to string mappings.
  // std::cout << "Initializing SDL event type to string mappings." << std::endl;
  event_type_to_string_[SDL_DOLLARGESTURE] = "dollarGesture";
  event_type_to_string_[SDL_DROPFILE] = "dropFile";
  event_type_to_string_[SDL_FINGERMOTION] = "fingerMotion";
  event_type_to_string_[SDL_FINGERDOWN] = "fingerDown";
  event_type_to_string_[SDL_FINGERUP] = "fingerUp";
  event_type_to_string_[SDL_KEYDOWN] = "keyDown";
  event_type_to_string_[SDL_KEYUP] = "keyUp";
  event_type_to_string_[SDL_JOYAXISMOTION] = "joyAxisMotion";
  event_type_to_string_[SDL_JOYBALLMOTION] = "joyBallMotion";
  event_type_to_string_[SDL_JOYHATMOTION] = "joyHatMotion";
  event_type_to_string_[SDL_JOYBUTTONDOWN] = "joyButtonDown";
  event_type_to_string_[SDL_JOYBUTTONUP] = "joyButtonUp";
  event_type_to_string_[SDL_MOUSEMOTION] = "mouseMotion";
  event_type_to_string_[SDL_MOUSEBUTTONDOWN] = "mouseButtonDown";
  event_type_to_string_[SDL_MOUSEBUTTONUP] = "mouseButtonUp";
  event_type_to_string_[SDL_MOUSEWHEEL] = "mouseWheel";
  event_type_to_string_[SDL_MULTIGESTURE] = "multiGesture";
  event_type_to_string_[SDL_QUIT] = "quit";
  event_type_to_string_[SDL_SYSWMEVENT] = "sysWMEvent";
  event_type_to_string_[SDL_TEXTEDITING] = "textEditing";
  event_type_to_string_[SDL_TEXTINPUT] = "textInput";
  event_type_to_string_[SDL_USEREVENT] = "userEvent";
  event_type_to_string_[SDL_WINDOWEVENT] = "windowEvent";
  // std::cout << "Finished initializing event mappings." << std::endl;

  // Initialize the SDL WindowEvent type to string mappings.
  // std::cout << "Initializing SDL window event type to string mappings." << std::endl;
  window_event_to_string_[SDL_WINDOWEVENT_SHOWN] = "shown";
  window_event_to_string_[SDL_WINDOWEVENT_HIDDEN] = "hidden";
  window_event_to_string_[SDL_WINDOWEVENT_EXPOSED] = "exposed";
  window_event_to_string_[SDL_WINDOWEVENT_MOVED] = "moved";
  window_event_to_string_[SDL_WINDOWEVENT_RESIZED] = "resized";
  window_event_to_string_[SDL_WINDOWEVENT_SIZE_CHANGED] = "sizeChanged";
  window_event_to_string_[SDL_WINDOWEVENT_MINIMIZED] = "minimized";
  window_event_to_string_[SDL_WINDOWEVENT_MAXIMIZED] = "maximized";
  window_event_to_string_[SDL_WINDOWEVENT_RESTORED] = "restored";
  window_event_to_string_[SDL_WINDOWEVENT_ENTER] = "enter";
  window_event_to_string_[SDL_WINDOWEVENT_LEAVE] = "leave";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_GAINED] = "focusGained";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_LOST] = "focusLost";
  window_event_to_string_[SDL_WINDOWEVENT_CLOSE] = "close";
  // std::cout << "Finished initializing window event mappings." << std::endl;

  sdl::InitWrappers(target);
  sdl::WindowWrapper::Init(target);
  sdl::RendererWrapper::Init(target);
  sdl::TextureWrapper::Init(target);
  sdl::SurfaceWrapper::Init(target);
  sdl::ColorWrapper::Init(target);
  sdl::RectWrapper::Init(target);
  sdl::gl::Init(target);
  sdl::event::Init(target);
  sdl::key::Init(target);
  sdl::mouse::Init(target);
  sdl::TTF::Initialize(target);
  sdl::joystick::Init(target);
  sdl::controller::Init(target);

  // Initialization and Shutdown.
  Nan::SetMethod(target, "init", sdl::Init);
  Nan::SetMethod(target, "initSubSystem", sdl::InitSubSystem);
  Nan::SetMethod(target, "wasInit", sdl::WasInit);
  Nan::SetMethod(target, "quit", sdl::Quit);
  Nan::SetMethod(target, "quitSubSystem", sdl::QuitSubSystem);

  // Display and Window Management.

  Nan::SetMethod(target, "clearError", sdl::ClearError);
  Nan::SetMethod(target, "getError", sdl::GetError);
  Nan::SetMethod(target, "setError", sdl::SetError);

  Nan::SetMethod(target, "mapRGB", sdl::MapRGB);
  Nan::SetMethod(target, "mapRGBA", sdl::MapRGBA);
  Nan::SetMethod(target, "getRGB", sdl::GetRGB);
  Nan::SetMethod(target, "getRGBA", sdl::GetRGBA);

  Nan::SetMethod(target, "AddHintCallback", sdl::AddHintCallback);
  Nan::SetMethod(target, "getHint", sdl::GetHint);
  Nan::SetMethod(target, "setHint", sdl::SetHint);
  Nan::SetMethod(target, "setHintWithPriority", sdl::SetHintWithPriority);

  Nan::SetMethod(target, "compiledVersion", sdl::CompiledVersion);
  Nan::SetMethod(target, "compiledRevision", sdl::CompiledRevision);
  Nan::SetMethod(target, "getRevision", sdl::GetRevision);
  Nan::SetMethod(target, "getRevisionNumber", sdl::GetRevisionNumber);
  Nan::SetMethod(target, "getVersion", sdl::GetVersion);

  Nan::SetMethod(target, "getClipboardText", sdl::GetClipboardText);
  Nan::SetMethod(target, "hasClipboardText", sdl::HasClipboardText);
  Nan::SetMethod(target, "setClipboardText", sdl::SetClipboardText);

  Local<Object> INIT = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("INIT").ToLocalChecked(), INIT);
  Nan::Set(INIT, Nan::New<String>("TIMER").ToLocalChecked(), Nan::New<Number>(SDL_INIT_TIMER));
  Nan::Set(INIT, Nan::New<String>("AUDIO").ToLocalChecked(), Nan::New<Number>(SDL_INIT_AUDIO));
  Nan::Set(INIT, Nan::New<String>("VIDEO").ToLocalChecked(), Nan::New<Number>(SDL_INIT_VIDEO));
  Nan::Set(INIT, Nan::New<String>("JOYSTICK").ToLocalChecked(), Nan::New<Number>(SDL_INIT_JOYSTICK));
  Nan::Set(INIT, Nan::New<String>("HAPTIC").ToLocalChecked(), Nan::New<Number>(SDL_INIT_HAPTIC));
  Nan::Set(INIT, Nan::New<String>("GAMECONTROLLER").ToLocalChecked(), Nan::New<Number>(SDL_INIT_GAMECONTROLLER));
  Nan::Set(INIT, Nan::New<String>("EVENTS").ToLocalChecked(), Nan::New<Number>(SDL_INIT_EVENTS));
  Nan::Set(INIT, Nan::New<String>("EVERYTHING").ToLocalChecked(), Nan::New<Number>(SDL_INIT_EVERYTHING));
  Nan::Set(INIT, Nan::New<String>("NOPARACHUTE").ToLocalChecked(), Nan::New<Number>(SDL_INIT_NOPARACHUTE));

  Local<Object> EVENT = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("EVENT").ToLocalChecked(), EVENT);
  Nan::Set(EVENT, Nan::New<String>("DOLLARGESTURE").ToLocalChecked(), Nan::New<Number>(SDL_DOLLARGESTURE));
  Nan::Set(EVENT, Nan::New<String>("DROPFILE").ToLocalChecked(), Nan::New<Number>(SDL_DROPFILE));
  Nan::Set(EVENT, Nan::New<String>("FINGERMOTION").ToLocalChecked(), Nan::New<Number>(SDL_FINGERMOTION));
  Nan::Set(EVENT, Nan::New<String>("FINGERUP").ToLocalChecked(), Nan::New<Number>(SDL_FINGERUP));
  Nan::Set(EVENT, Nan::New<String>("KEYDOWN").ToLocalChecked(), Nan::New<Number>(SDL_KEYDOWN));
  Nan::Set(EVENT, Nan::New<String>("KEYUP").ToLocalChecked(), Nan::New<Number>(SDL_KEYUP));
  Nan::Set(EVENT, Nan::New<String>("JOYAXISMOTION").ToLocalChecked(), Nan::New<Number>(SDL_JOYAXISMOTION));
  Nan::Set(EVENT, Nan::New<String>("JOYBALLMOTION").ToLocalChecked(), Nan::New<Number>(SDL_JOYBALLMOTION));
  Nan::Set(EVENT, Nan::New<String>("JOYHATMOTION").ToLocalChecked(), Nan::New<Number>(SDL_JOYHATMOTION));
  Nan::Set(EVENT, Nan::New<String>("JOYBUTTONDOWN").ToLocalChecked(), Nan::New<Number>(SDL_JOYBUTTONDOWN));
  Nan::Set(EVENT, Nan::New<String>("JOYBUTTONUP").ToLocalChecked(), Nan::New<Number>(SDL_JOYBUTTONUP));
  Nan::Set(EVENT, Nan::New<String>("MOUSEMOTION").ToLocalChecked(), Nan::New<Number>(SDL_MOUSEMOTION));
  Nan::Set(EVENT, Nan::New<String>("MOUSEBUTTONDOWN").ToLocalChecked(), Nan::New<Number>(SDL_MOUSEBUTTONDOWN));
  Nan::Set(EVENT, Nan::New<String>("MOUSEBUTTONUP").ToLocalChecked(), Nan::New<Number>(SDL_MOUSEBUTTONUP));
  Nan::Set(EVENT, Nan::New<String>("MOUSEWHEEL").ToLocalChecked(), Nan::New<Number>(SDL_MOUSEWHEEL));
  Nan::Set(EVENT, Nan::New<String>("MULTIGESTURE").ToLocalChecked(), Nan::New<Number>(SDL_MULTIGESTURE));
  Nan::Set(EVENT, Nan::New<String>("QUIT").ToLocalChecked(), Nan::New<Number>(SDL_QUIT));
  Nan::Set(EVENT, Nan::New<String>("SYSWMEVENT").ToLocalChecked(), Nan::New<Number>(SDL_SYSWMEVENT));
  Nan::Set(EVENT, Nan::New<String>("TEXTEDITING").ToLocalChecked(), Nan::New<Number>(SDL_TEXTEDITING));
  Nan::Set(EVENT, Nan::New<String>("TEXTINPUT").ToLocalChecked(), Nan::New<Number>(SDL_TEXTINPUT));
  Nan::Set(EVENT, Nan::New<String>("USEREVENT").ToLocalChecked(), Nan::New<Number>(SDL_USEREVENT));
  Nan::Set(EVENT, Nan::New<String>("WINDOWEVENT").ToLocalChecked(), Nan::New<Number>(SDL_WINDOWEVENT));

  // SDL Enumerations start:

  Local<Object> AUDIOFORMAT = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("AUDIOFORMAT").ToLocalChecked(), AUDIOFORMAT);
  Nan::Set(AUDIOFORMAT, Nan::New<String>("MASK_BITSIZE").ToLocalChecked(), Nan::New<Number>(SDL_AUDIO_MASK_BITSIZE));
  Nan::Set(AUDIOFORMAT, Nan::New<String>("MASK_DATATYPE").ToLocalChecked(), Nan::New<Number>(SDL_AUDIO_MASK_DATATYPE));
  Nan::Set(AUDIOFORMAT, Nan::New<String>("MASK_ENDIAN").ToLocalChecked(), Nan::New<Number>(SDL_AUDIO_MASK_ENDIAN));
  Nan::Set(AUDIOFORMAT, Nan::New<String>("MASK_SIGNED").ToLocalChecked(), Nan::New<Number>(SDL_AUDIO_MASK_SIGNED));

  Local<Object> TEXTUREACCESS = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("TEXTUREACCESS").ToLocalChecked(), TEXTUREACCESS);
  Nan::Set(TEXTUREACCESS, Nan::New<String>("STATIC").ToLocalChecked(), Nan::New<Number>(SDL_TEXTUREACCESS_STATIC));
  Nan::Set(TEXTUREACCESS, Nan::New<String>("STREAMING").ToLocalChecked(), Nan::New<Number>(SDL_TEXTUREACCESS_STREAMING));

  Local<Object> IMG = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("IMG").ToLocalChecked(), IMG);

  Nan::SetMethod(IMG, "load", sdl::IMG::Load);

  Local<Object> WM = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("WM").ToLocalChecked(), WM);

  Local<Object> HINT = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("HINT").ToLocalChecked(), HINT);
  Nan::Set(HINT, Nan::New<String>("FRAMEBUFFER_ACCELERATION").ToLocalChecked(), Nan::New<String>(SDL_HINT_FRAMEBUFFER_ACCELERATION).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("IDLE_TIMER_DISABLED").ToLocalChecked(), Nan::New<String>(SDL_HINT_IDLE_TIMER_DISABLED).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("ORIENTATIONS").ToLocalChecked(), Nan::New<String>(SDL_HINT_ORIENTATIONS).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("RENDER_DRIVER").ToLocalChecked(), Nan::New<String>(SDL_HINT_RENDER_DRIVER).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("RENDER_OPENGL_SHADERS").ToLocalChecked(), Nan::New<String>(SDL_HINT_RENDER_OPENGL_SHADERS).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("SCALE_QUALITY").ToLocalChecked(), Nan::New<String>(SDL_HINT_RENDER_SCALE_QUALITY).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("RENDER_VSYNC").ToLocalChecked(), Nan::New<String>(SDL_HINT_RENDER_VSYNC).ToLocalChecked());
  Nan::Set(HINT, Nan::New<String>("DEFAULT").ToLocalChecked(), Nan::New<Number>(SDL_HINT_DEFAULT));
  Nan::Set(HINT, Nan::New<String>("NORMAL").ToLocalChecked(), Nan::New<Number>(SDL_HINT_NORMAL));
  Nan::Set(HINT, Nan::New<String>("OVERRIDE").ToLocalChecked(), Nan::New<Number>(SDL_HINT_OVERRIDE));

  Local<Object> RENDERER = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("RENDERER").ToLocalChecked(), RENDERER);
  Nan::Set(RENDERER, Nan::New<String>("SOFTWARE").ToLocalChecked(), Nan::New<Number>(SDL_RENDERER_SOFTWARE));
  Nan::Set(RENDERER, Nan::New<String>("ACCELERATED").ToLocalChecked(), Nan::New<Number>(SDL_RENDERER_ACCELERATED));
  Nan::Set(RENDERER, Nan::New<String>("PRESENTVSYNC").ToLocalChecked(), Nan::New<Number>(SDL_RENDERER_PRESENTVSYNC));
  Nan::Set(RENDERER, Nan::New<String>("TARGETTEXTURE").ToLocalChecked(), Nan::New<Number>(SDL_RENDERER_TARGETTEXTURE));

  Local<Object> BLENDMODE = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("BLENDMODE").ToLocalChecked(), BLENDMODE);
  Nan::Set(BLENDMODE, Nan::New<String>("NONE").ToLocalChecked(), Nan::New<Number>(SDL_BLENDMODE_NONE));
  Nan::Set(BLENDMODE, Nan::New<String>("BLEND").ToLocalChecked(), Nan::New<Number>(SDL_BLENDMODE_BLEND));
  Nan::Set(BLENDMODE, Nan::New<String>("ADD").ToLocalChecked(), Nan::New<Number>(SDL_BLENDMODE_ADD));
  Nan::Set(BLENDMODE, Nan::New<String>("MOD").ToLocalChecked(), Nan::New<Number>(SDL_BLENDMODE_MOD));

  Local<Object> FLIP = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("FLIP").ToLocalChecked(), FLIP);
  Nan::Set(FLIP, Nan::New<String>("NONE").ToLocalChecked(), Nan::New<Number>(SDL_FLIP_NONE));
  Nan::Set(FLIP, Nan::New<String>("HORIZONTAL").ToLocalChecked(), Nan::New<Number>(SDL_FLIP_HORIZONTAL));
  Nan::Set(FLIP, Nan::New<String>("VERTICAL").ToLocalChecked(), Nan::New<Number>(SDL_FLIP_VERTICAL));

  Local<Object> WINDOWPOS = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("WINDOWPOS").ToLocalChecked(), WINDOWPOS);
  Nan::Set(WINDOWPOS, Nan::New<String>("CENTERED").ToLocalChecked(), Nan::New<Number>(SDL_WINDOWPOS_CENTERED));
  Nan::Set(WINDOWPOS, Nan::New<String>("UNDEFINED").ToLocalChecked(), Nan::New<Number>(SDL_WINDOWPOS_UNDEFINED));

  Local<Object> WINDOW = Nan::New<Object>();
  Nan::Set(target, Nan::New<String>("WINDOW").ToLocalChecked(), WINDOW);
  Nan::Set(WINDOW, Nan::New<String>("FULLSCREEN").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_FULLSCREEN));
  Nan::Set(WINDOW, Nan::New<String>("FULLSCREEN_DESKTOP").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_FULLSCREEN_DESKTOP));
  Nan::Set(WINDOW, Nan::New<String>("OPENGL").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_OPENGL));
  Nan::Set(WINDOW, Nan::New<String>("HIDDEN").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_HIDDEN));
  Nan::Set(WINDOW, Nan::New<String>("BORDERLESS").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_BORDERLESS));
  Nan::Set(WINDOW, Nan::New<String>("RESIZABLE").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_RESIZABLE));
  Nan::Set(WINDOW, Nan::New<String>("MINIMIZED").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_MINIMIZED));
  Nan::Set(WINDOW, Nan::New<String>("MAXIMIZED").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_MAXIMIZED));
  Nan::Set(WINDOW, Nan::New<String>("INPUT_GRABBED").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_INPUT_GRABBED));
  Nan::Set(WINDOW, Nan::New<String>("ALLOW_HIGHDPI").ToLocalChecked(), Nan::New<Number>(SDL_WINDOW_ALLOW_HIGHDPI));
}


////////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown.

NAN_METHOD(sdl::Init) {
  SDL_SetMainReady();
  int init = (info[0]->IsUndefined() || !info[0]->IsNumber()) ? SDL_INIT_EVERYTHING : info[0]->Int32Value();
  // std::cout << "sdl::Init got: " << init << std::endl;
  if (SDL_Init(init) < 0) {
    return ThrowSDLException(__func__);
  }
}

NAN_METHOD(sdl::InitSubSystem) {

  if (!(info.Length() == 1 && info[0]->IsNumber())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected InitSubSystem(Number)").ToLocalChecked()));
  }

  if (SDL_InitSubSystem(info[0]->Int32Value()) < 0) return ThrowSDLException(__func__);
}

NAN_METHOD(sdl::Quit) {

  if (!(info.Length() == 0)) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected Quit()").ToLocalChecked()));
  }

  SDL_Quit();
}

NAN_METHOD(sdl::QuitSubSystem) {

  if (!(info.Length() == 1 && info[0]->IsNumber())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected QuitSubSystem(Number)").ToLocalChecked()));
  }

  SDL_QuitSubSystem(info[0]->Int32Value());
}

NAN_GETTER(sdl::WasInit) {

  if (!(info.Length() == 1 && info[0]->IsNumber())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected WasInit(Number)").ToLocalChecked()));
  }

  return Nan::New<Number>(SDL_WasInit(info[0]->Int32Value()));
}

////////////////////////////////////////////////////////////////////////////////
// Display and Window Management.


////////////////////////////////////////////////////////////////////////////////
// Global SDL functions.
NAN_METHOD(sdl::DisableScreenSaver) {
  SDL_DisableScreenSaver();
}
NAN_METHOD(sdl::EnableScreenSaver) {
  SDL_EnableScreenSaver();
}
NAN_GETTER(sdl::IsScreenSaverEnabled) {

  SDL_bool ret = SDL_IsScreenSaverEnabled();
  return scope.Close(Nan::New<Boolean>(ret));
}

NAN_GETTER(sdl::GetClosestDisplayMode) {

  if(info.Length() < 2) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected getClosestDisplayMode(Number, DisplayMode)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  SDL_DisplayMode* mode = UnwrapDisplayMode(Handle<Object>::Cast(info[1]));
  SDL_DisplayMode* closest = new SDL_DisplayMode;
  SDL_DisplayMode* err = SDL_GetClosestDisplayMode(index, mode, closest);
  if(NULL == err) {
    delete closest;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(closest));
}
NAN_GETTER(sdl::GetCurrentDisplayMode) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected getCurrentDisplayMode(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  SDL_DisplayMode* current = new SDL_DisplayMode;
  int err = SDL_GetCurrentDisplayMode(index, current);
  if(err < 0) {
    delete current;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(current));
}
NAN_GETTER(sdl::GetCurrentVideoDriver) {

  const char* ret = SDL_GetCurrentVideoDriver();
  if(NULL == ret) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(Nan::New<String>(ret));
}
NAN_GETTER(sdl::GetDesktopDisplayMode) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected getCurrentDisplayMode(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  SDL_DisplayMode* current = new SDL_DisplayMode;
  int err = SDL_GetDesktopDisplayMode(index, current);
  if(err < 0) {
    delete current;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(current));
}
NAN_GETTER(sdl::GetDisplayBounds) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected getDisplayBounds(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  SDL_Rect* bounds = new SDL_Rect;
  int err = SDL_GetDisplayBounds(index, bounds);
  if(err < 0) {
    delete bounds;
    return ThrowSDLException(__func__);
  }

  NEW_WRAPPED(bounds, RectWrapper, ret);
//   Handle<Value> argv[] = {External::New(bounds)};
//   Handle<Object> ret = RectWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
//   Handle<Object> ret = Nan::New<Object>();
//   RectWrapper* wrap = new RectWrapper(ret);
//   wrap->wrapped = bounds;

  return scope.Close(ret);
}
NAN_GETTER(sdl::GetDisplayMode) {

  if(info.Length() < 2) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected getDisplayMode(Number, Number)").ToLocalChecked()));
  }

  int displayIndex = info[0]->Int32Value();
  int modeIndex = info[1]->Int32Value();
  SDL_DisplayMode* mode = new SDL_DisplayMode;
  int err = SDL_GetDisplayMode(displayIndex, modeIndex, mode);
  if(err < 0) {
    delete mode;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(mode));
}
NAN_GETTER(sdl::GetDisplayName) {

  if(info.Length() < 0) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: expected getDisplayName(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  const char* ret = SDL_GetDisplayName(index);
  if(NULL == ret) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(Nan::New<String>(ret));
}
NAN_GETTER(sdl::GetNumDisplayModes) {

  if(info.Length() < 0) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: expected getNumDisplayModes(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  int ret = SDL_GetNumDisplayModes(index);
  if(ret < 0) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(Nan::New<Number>(ret));
}
NAN_GETTER(sdl::GetNumVideoDisplays) {

  int num = SDL_GetNumVideoDisplays();
  if(num < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Nan::New<Number>(num));
}
NAN_GETTER(sdl::GetNumVideoDrivers) {

  int num = SDL_GetNumVideoDrivers();
  if(num < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Nan::New<Number>(num));
}
NAN_GETTER(sdl::GetVideoDriver) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: expected getVideoDriver(Number)").ToLocalChecked()));
  }

  int index = info[0]->Int32Value();
  const char* driver = SDL_GetVideoDriver(index);
  if(NULL == driver) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Nan::New<String>(driver));
}

// NAN_GETTER(sdl::ShowMessageBox) {

// }
NAN_METHOD(sdl::ShowSimpleMessageBox) {

  if(info.Length() < 3) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: expected showSimpleMessageBox(Number, String, String, [Window])").ToLocalChecked()));
  }
  else if(info.Length() < 4) {
    int flags = info[0]->Int32Value();
    String::Utf8Value title(info[1]);
    String::Utf8Value message(info[2]);
    int err = SDL_ShowSimpleMessageBox(flags, *title, *message, NULL);
    if(err < 0) {
      return ThrowSDLException(__func__);
    }
  }
  else {
    int flags = info[0]->Int32Value();
    String::Utf8Value title(info[1]);
    String::Utf8Value message(info[2]);
    WindowWrapper* window = node::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(info[3]));
    int err = SDL_ShowSimpleMessageBox(flags, *title, *message, window->window_);
    if(err < 0) {
      return ThrowSDLException(__func__);
    }
  }

  return Nan::Undefined();
}

NAN_GETTER(sdl::VideoInit) {

  const char* driver = info[0]->IsUndefined() ? NULL : *(String::Utf8Value(info[0]));
  int err = SDL_VideoInit(driver);
  if(err < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Nan::New<String>(driver));
}

NAN_METHOD(sdl::VideoQuit) {
  SDL_VideoQuit();
}

////////////////////////////////////////////////////////////////////////////////

NAN_METHOD(sdl::ClearError) {

  if (!(info.Length() == 0)) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected ClearError()").ToLocalChecked()));
  }

  SDL_ClearError();
}

NAN_GETTER(sdl::GetError) {

  if (!(info.Length() == 0)) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected GetError()").ToLocalChecked()));
  }

  return Nan::New<String>(SDL_GetError());
}

NAN_SETTER(sdl::SetError) {

  if (!(info.Length() == 1 && info[0]->IsString())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected SetError(String)").ToLocalChecked()));
  }

  String::Utf8Value message(info[1]);

  SDL_SetError(*message);
}

NAN_GETTER(sdl::MapRGB) {

  if (!(info.Length() == 4 && info[0]->IsObject() && info[1]->IsNumber() && info[2]->IsNumber() && info[3]->IsNumber())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected MapRGB(PixelFormat, Number, Number, Number)").ToLocalChecked()));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(info[0]->ToObject());
  int r = info[1]->Int32Value();
  int g = info[2]->Int32Value();
  int b = info[3]->Int32Value();

  return Nan::New<Number>(SDL_MapRGB(fmt, r, g, b));
}

NAN_GETTER(sdl::MapRGBA) {

  if (!(info.Length() == 5 && info[0]->IsObject() && info[1]->IsNumber() && info[2]->IsNumber() && info[3]->IsNumber() && info[4]->IsNumber())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected MapRGBA(PixelFormat, Number, Number, Number, Number)").ToLocalChecked()));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(info[0]->ToObject());
  int r = info[1]->Int32Value();
  int g = info[2]->Int32Value();
  int b = info[3]->Int32Value();
  int a = info[4]->Int32Value();

  return Nan::New<Number>(SDL_MapRGBA(fmt, r, g, b, a));
}

NAN_GETTER(sdl::GetRGB) {

  if (!(info.Length() == 2 && info[0]->IsNumber() && info[1]->IsObject())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected GetRGB(Number, PixelFormat)").ToLocalChecked()));
  }

  int pixel = info[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(info[1]->ToObject());
  ::Uint8 r, g, b;

  SDL_GetRGB(pixel, fmt, &r, &g, &b);

  Local<Object> rgb = Nan::New<Object>();
  Nan::Set(rgb, Nan::New<String>("r").ToLocalChecked(), Nan::New<Number>(r));
  Nan::Set(rgb, Nan::New<String>("g").ToLocalChecked(), Nan::New<Number>(g));
  Nan::Set(rgb, Nan::New<String>("b").ToLocalChecked(), Nan::New<Number>(b));

  return scope.Close(rgb);
}

NAN_GETTER(sdl::GetRGBA) {

  if (!(info.Length() == 2 && info[0]->IsNumber() && info[1]->IsObject())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected GetRGBA(Number, PixelFormat)").ToLocalChecked()));
  }

  int pixel = info[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(info[1]->ToObject());
  ::Uint8 r, g, b, a;

  SDL_GetRGBA(pixel, fmt, &r, &g, &b, &a);

  Local<Object> rgba = Nan::New<Object>();
  Nan::Set(rgba, Nan::New<String>("r").ToLocalChecked(), Nan::New<Number>(r));
  Nan::Set(rgba, Nan::New<String>("g").ToLocalChecked(), Nan::New<Number>(g));
  Nan::Set(rgba, Nan::New<String>("b").ToLocalChecked(), Nan::New<Number>(b));
  Nan::Set(rgba, Nan::New<String>("a").ToLocalChecked(), Nan::New<Number>(a));

  return scope.Close(rgba);
}

////////////////////////////////////////////////////////////////////////////////
// SDL Hint Handling.
static void HintCallbackHandler(void *userData, const char *name, const char *oldValue, const char *newValue) {

  Persistent<Function> callback = *static_cast<Persistent<Function>*>(userData);

  Local<Value> nodeName = Nan::New<String>(name);
  Local<Value> nodeOldValue = Nan::New<String>(oldValue);
  Local<Value> nodeNewValue = Nan::New<String>(newValue);

  Local<Value> argv[3] = {nodeName, nodeOldValue, nodeNewValue};
  Local<Value> retValue = callback->Call(Context::GetCurrent()->Global(), 3, argv);
  Local<Boolean> ret = retValue->ToBoolean();
  if(ret->BooleanValue()) {
    SDL_DelHintCallback(name, HintCallbackHandler, userData);
    callback.Dispose();
  }
}

NAN_METHOD(sdl::AddHintCallback) {

  if(info.Length() < 2) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("Invalid arguments: Expected AddHintCallback(String, Function)").ToLocalChecked()));
  }

  String::Utf8Value name(info[0]);
  Handle<Function> callback = Handle<Function>::Cast(info[1]);
  Persistent<Function> userData = Persistent<Function>::New(callback);
  SDL_AddHintCallback(*name, HintCallbackHandler, static_cast<void*>(&userData));
}
// TODO: Implement a way to call SDL_ClearHints safely. Currently, because we store a Persistent
//       in the userData slot, this would leak memory due to those functions never being cleaned
//       up. Need some global list of Persistents to dispose of when this is called.
// TODO: Also implement a wrapper around SDL_DelHintCallback. (need to return a token or something
//       when adding a callback, because it's likely most callbacks will be anonymous so we won't
//       have the exact same Persistent address)
// NAN_METHOD(ClearHints) {
//
//   return Nan::Undefined();
// }
NAN_GETTER(sdl::GetHint) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("Invalid arguments: Expected GetHint(String)").ToLocalChecked()));
  }

  String::Utf8Value name(info[0]);
  const char *value = SDL_GetHint(*name);
  if(NULL == value) {
    return Nan::Undefined();
  }

  return scope.Close(Nan::New<String>(value));
}
NAN_SETTER(sdl::SetHint) {

  if(info.Length() < 2) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("Invalid arguments: Expected SetHint(String, String)").ToLocalChecked()));
  }

  String::Utf8Value name(info[0]);
  String::Utf8Value value(info[1]);
  int err = SDL_SetHint(*name, *value);
  if(err < 0) {
    return ThrowSDLException(__func__);
  }

  return Nan::Undefined();
}
NAN_SETTER(sdl::SetHintWithPriority) {

  if(info.Length() < 3) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("Invalid arguments: Excpected SetHintWithPriority(String, String, Number)").ToLocalChecked()));
  }

  String::Utf8Value name(info[0]);
  String::Utf8Value value(info[1]);
  SDL_HintPriority priority = static_cast<SDL_HintPriority>(info[2]->Int32Value());
  SDL_bool ret = SDL_SetHintWithPriority(*name, *value, priority);

  return scope.Close(Nan::New<Boolean>(ret ? true : false));
}

NAN_GETTER(sdl::CompiledVersion) {

  SDL_version version;
  SDL_VERSION(&version);

  Handle<Object> ret = Nan::New<Object>();
  Nan::Set(ret, Nan::New<String>("major").ToLocalChecked(), Nan::New<Number>(version.major));
  Nan::Set(ret, Nan::New<String>("minor").ToLocalChecked(), Nan::New<Number>(version.minor));
  Nan::Set(ret, Nan::New<String>("patch").ToLocalChecked(), Nan::New<Number>(version.patch));

  return scope.Close(ret);
}
NAN_GETTER(sdl::CompiledRevision) {

  Handle<String> ret = Nan::New<String>(SDL_REVISION);

  return scope.Close(ret);
}
NAN_GETTER(sdl::GetRevision) {

  const char *revision = SDL_GetRevision();
  Handle<String> ret = Nan::New<String>(revision);

  return scope.Close(ret);
}
NAN_GETTER(sdl::GetRevisionNumber) {

  int revision = SDL_GetRevisionNumber();
  Handle<Value> ret = Nan::New<Number>(revision);

  return scope.Close(ret);
}
NAN_GETTER(sdl::GetVersion) {

  SDL_version version;
  SDL_GetVersion(&version);

  Handle<Object> ret = Nan::New<Object>();
  Nan::Set(ret, Nan::New<String>("major").ToLocalChecked(), Nan::New<Number>(version.major));
  Nan::Set(ret, Nan::New<String>("minor").ToLocalChecked(), Nan::New<Number>(version.minor));
  Nan::Set(ret, Nan::New<String>("patch").ToLocalChecked(), Nan::New<Number>(version.patch));

  return scope.Close(ret);
}

NAN_GETTER(sdl::GetClipboardText) {

  char *text = SDL_GetClipboardText();
  if(NULL == text) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(Nan::New<String>(text));
}
NAN_GETTER(sdl::HasClipboardText) {

  SDL_bool has = SDL_HasClipboardText();

  return scope.Close(Nan::New<Boolean>(has ? true : false));
}
NAN_SETTER(sdl::SetClipboardText) {

  if(info.Length() < 1) {
    return Nan::ThrowError(Exception::TypeError(
      Nan::New<String>("Invalid arguments: Expected SetClipboardText(String)")));
  }

  String::Utf8Value text(info[0]);
  int err = SDL_SetClipboardText(*text);
  if(err < 0) {
    return ThrowSDLException(__func__);
  }
}


// TODO: make an async version so this can be used in loops or parallel load images
NAN_GETTER(sdl::IMG::Load) {

  if (!(info.Length() == 1 && info[0]->IsString())) {
    return Nan::ThrowError(Exception::TypeError(Nan::New<String>("Invalid arguments: Expected IMG::Load(String)")));
  }

  String::Utf8Value file(info[0]);

  SDL_Surface *image;
  image=IMG_Load(*file);
  if(!image) {
    return Nan::ThrowError(Exception::Error(String::Concat(
      Nan::New<String>("IMG::Load: "),
      Nan::New<String>(IMG_GetError())
    )));
  }

  NEW_WRAPPED(image, SurfaceWrapper, ret)

  return scope.Close(ret);
}

NODE_MODULE(node_sdl, init)
