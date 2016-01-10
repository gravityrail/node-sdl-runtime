#ifndef NODEJS_FONT_H
#define NODEJS_FONT_H

#include <v8.h>
#include <node.h>

#include "SDL.h"
#include "SDL_ttf.h"


namespace sdl {
	namespace TTF {
		class FontWrapper : public Nan::ObjectWrap {
		public:
			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			FontWrapper();
			~FontWrapper();

			static void Init(v8::Handle<v8::Object> exports);
			static NAN_METHOD(New);

			static NAN_METHOD(RenderTextSolid);
			static NAN_METHOD(RenderUTF8Solid);
			static NAN_METHOD(RenderUnicodeSolid);
			static NAN_METHOD(RenderGlyphSolid);

			static NAN_METHOD(RenderTextShaded);
			static NAN_METHOD(RenderUTF8Shaded);
			static NAN_METHOD(RenderUnicodeShaded);
			static NAN_METHOD(RenderGlyphShaded);

			static NAN_METHOD(RenderTextBlended);
			static NAN_METHOD(RenderUTF8Blended);
			static NAN_METHOD(RenderUnicodeBlended);
			static NAN_METHOD(RenderGlyphBlended);

			TTF_Font* font_;
		};

		void Initialize(v8::Handle<v8::Object> exports);

		NAN_METHOD(Init);
		NAN_METHOD(WasInit);
		NAN_METHOD(Quit);

		// NAN_METHOD(SetError);
		NAN_METHOD(GetError);

		// NAN_METHOD(OpenFont);
		// NAN_METHOD(OpenFontIndex);
		// NAN_METHOD(CloseFont);
	}
}

#endif
