#ifndef NODE_SDL_H_
#define NODE_SDL_H_

// Various defines to modify how the bindings operate somewhat when compiled. This can include
// things such as disabling runtime argument checks, including various logging, etc.
//
// NOTE: None of these flags are actually used yet. I am only putting them in here as a note to
// myself as to what features I will want to be optionally disabled in the future. Otherwise I
// will forget :)

// NOTE: All of these defines are actually put into binding.gyp
// WARNING: The only time you should disable arg checking is after thoroughly testing your application
// and you don't see any exceptions being thrown. You WILL get segfaults if you aren't careful about
// what types you pass to some functions without having them checked. (such as passing an SDL_Color
// when something want an SDL_Texture, or vise versa) And there will be no stacktrace or anything
// pointing to a location where the problem happened.
// #define ENABLE_ARG_CHECKING
// If this is not defined, the entire SDL_image wrapper will not be included in the build.
// #define ENABLE_IMAGE
// If this is not defined, the entire SDL_ttf wrapper will not be included in the build.
// #define ENABLE_TTF

#include <node.h>
#include <v8.h>
#include <map>
#include <string>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "helpers.h"
#include "render.h"

namespace sdl {
  // Initialization and Shutdown.
  NAN_METHOD(Init);
  NAN_METHOD(InitSubSystem);
  NAN_METHOD(WasInit);
  NAN_METHOD(Quit);
  NAN_METHOD(QuitSubSystem);

  // Display and Window Management.
  NAN_METHOD(DisableScreenSaver);
  NAN_METHOD(EnableScreenSaver);
  NAN_GETTER(IsScreenSaverEnabled);

  NAN_GETTER(GetClosestDisplayMode);
  NAN_GETTER(GetCurrentDisplayMode);
  NAN_GETTER(GetCurrentVideoDriver);
  NAN_GETTER(GetDesktopDisplayMode);
  NAN_GETTER(GetDisplayBounds);
  NAN_GETTER(GetDisplayMode);
  NAN_GETTER(GetDisplayName);
  NAN_GETTER(GetNumDisplayModes);
  NAN_GETTER(GetNumVideoDisplays);
  NAN_GETTER(GetNumVideoDrivers);
  NAN_GETTER(GetVideoDriver);
  // TODO: Put this functionality into Window?
  // NAN_METHOD(GetWindowFromID);

  // TODO: Implement showing a normal message box.
  // NAN_METHOD(ShowMessageBox);
  NAN_METHOD(ShowSimpleMessageBox);

  NAN_METHOD(VideoInit);
  NAN_METHOD(VideoQuit);

  // Error handling?
  NAN_METHOD(ClearError);
  NAN_GETTER(GetError);
  NAN_SETTER(SetError);

  NAN_METHOD(MapRGB);
  NAN_METHOD(MapRGBA);
  NAN_GETTER(GetRGB);
  NAN_GETTER(GetRGBA);

  NAN_METHOD(AddHintCallback);
  // NAN_METHOD(ClearHints);
  NAN_GETTER(GetHint);
  NAN_SETTER(SetHint);
  NAN_SETTER(SetHintWithPriority);

  NAN_METHOD(CompiledVersion);
  NAN_METHOD(CompiledRevision);
  NAN_GETTER(GetRevision);
  NAN_GETTER(GetRevisionNumber);
  NAN_GETTER(GetVersion);

  NAN_GETTER(GetClipboardText);
  NAN_GETTER(HasClipboardText);
  NAN_SETTER(SetClipboardText);

  namespace IMG {
    NAN_METHOD(Load);
  }

  namespace WM {
  }
}

#endif
